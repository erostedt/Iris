#include <array>
#include <memory>
#include <vector>

#include <glm/glm.hpp>

struct BoundingBox
{
    glm::vec3 min, max;
    BoundingBox(const glm::vec3 &_min, const glm::vec3 &_max) : min(_min), max(_max){};
    BoundingBox(float minx, float maxx, float miny, float maxy, float minz, float maxz)
        : min({minx, miny, minz}), max({maxx, maxy, maxz}){};

    bool contains(const glm::vec3 &point)
    {
        return (point.x >= min.x && point.x <= max.x && point.y >= min.y && point.y <= max.y && point.z >= min.z &&
                point.z <= max.z);
    }

    bool intersects(const BoundingBox &other)
    {
        return (min.x <= other.max.x && max.x >= other.min.x && min.y <= other.max.y && max.y >= other.min.y &&
                min.z <= other.max.z && max.z >= other.min.z);
    }
    glm::vec3 center()
    {
        return 0.5f * (min + max);
    }
    glm::vec3 dimensions()
    {
        return max - min;
    }
    BoundingBox padded(float xscale, float yscale, float zscale)
    {
        glm::vec3 ctr = center();
        glm::vec3 dims = dimensions();

        glm::vec3 new_dims = {dims.x * xscale, dims.y * yscale, dims.z * zscale};
        return {ctr - 0.5f * new_dims, ctr + 0.5f * new_dims};
    }
};

struct Voxel
{
    glm::vec3 center;
    glm::vec3 dimensions;
};

class Octree
{
  private:
    struct Node
    {
        BoundingBox bounds;
        size_t level{0};
        std::vector<glm::vec3> points;
        std::array<std::unique_ptr<Node>, 8> children;
        Node(const BoundingBox &_bounds) : bounds(_bounds), level(0)
        {
        }
        Node(const BoundingBox &_bounds, size_t _level) : bounds(_bounds), level(_level)
        {
        }
        void split()
        {
            glm::vec3 min = bounds.min;
            glm::vec3 max = bounds.max;
            glm::vec3 center = bounds.center();

            children[0] = std::make_unique<Node>(BoundingBox(min, center), level + 1);
            children[1] = std::make_unique<Node>(
                BoundingBox(glm::vec3(center.x, min.y, min.z), glm::vec3(max.x, center.y, center.z)), level + 1);
            children[2] = std::make_unique<Node>(
                BoundingBox(glm::vec3(min.x, min.y, center.z), glm::vec3(center.x, center.y, max.z)), level + 1);
            children[3] = std::make_unique<Node>(
                BoundingBox(glm::vec3(center.x, min.y, center.z), glm::vec3(max.x, center.y, max.z)), level + 1);
            children[4] = std::make_unique<Node>(
                BoundingBox(glm::vec3(min.x, center.y, min.z), glm::vec3(center.x, max.y, center.z)), level + 1);
            children[5] = std::make_unique<Node>(
                BoundingBox(glm::vec3(center.x, center.y, min.z), glm::vec3(max.x, max.y, center.z)), level + 1);
            children[6] = std::make_unique<Node>(
                BoundingBox(glm::vec3(min.x, center.y, center.z), glm::vec3(center.x, max.y, max.z)), level + 1);
            children[7] = std::make_unique<Node>(BoundingBox(center, max), level + 1);

            for (const glm::vec3 &point : points)
            {
                for (const auto &child : children)
                {
                    if (child->bounds.contains(point))
                    {
                        child->points.push_back(point);
                        break;
                    }
                }
            }
            points.clear();
        }
        bool is_leaf()
        {
            return children.at(0) == nullptr;
        }

        bool has_points()
        {
            return points.size() > 0;
        }
    };

  public:
    Octree(const BoundingBox &bounds, size_t max_points_per_node) : m_max_points_per_node(max_points_per_node)
    {
        m_root = std::make_unique<Node>(bounds);
    }

    Octree(const BoundingBox &bounds, size_t max_points_per_node, const std::vector<glm::vec3> &points)
        : Octree(bounds, max_points_per_node)
    {
        for (const auto &point : points)
        {
            insert(point);
        }
    }

    void insert(const glm::vec3 &point)
    {
        insert(point, m_root.get());
    }

    std::vector<Voxel> voxelize(size_t min_leaf_points = 1, size_t min_level = 0)
    {
        std::vector<Voxel> voxels;
        voxelize(m_root.get(), min_leaf_points, min_level, voxels);
        return voxels;
    }

  private:
    void voxelize(Node *node, size_t min_leaf_points, size_t min_level, std::vector<Voxel> &out_voxels)
    {
        if (!node->is_leaf())
        {
            for (const auto &child : node->children)
            {
                voxelize(child.get(), min_leaf_points, min_level, out_voxels);
            }
            return;
        }

        if (node->is_leaf() && node->points.size() >= min_leaf_points && node->level >= min_level)
        {
            out_voxels.push_back({node->bounds.center(), node->bounds.dimensions()});
        }
    }

    void insert(const glm::vec3 &point, Node *node)
    {
        if (!node->bounds.contains(point))
        {
            return;
        }

        if (node->points.size() < m_max_points_per_node)
        {
            node->points.push_back(point);
            return;
        }

        if (node->is_leaf())
        {
            node->split();
        }

        for (const auto &child : node->children)
        {
            insert(point, child.get());
        }
    }

  private:
    size_t m_max_points_per_node;
    std::unique_ptr<Node> m_root;
};
