#include <iostream>
#include <queue>
#include <set>
#include <optional>
#include <cassert>

namespace dnc {

template <class DataType, class TagType>
struct SplayNode {
    struct TagReference {
        SplayNode* node;
        TagType value;
    };

    SplayNode *left, *right;
    SplayNode* parent;
    unsigned size;
    std::multiset<TagType> tags;
    std::optional<TagReference> subtree_tag;
    DataType data;

    std::optional<TagReference> get_tag() { 
        splay();
        return subtree_tag;
    }

    /// @brief Remove the tag from the node, if it exists
    /// @param tg The tag to remove
    /// @return True if the tag was removed, false otherwise
    bool pop_tag(TagType tg) {
        auto ptr = tags.find(tg);
        if(ptr==tags.end()){
            return false;
        }
        tags.erase(ptr);
        splay();
        return true;
    }

    /// @brief Get the rightmost node in the tree
    SplayNode* get_rightmost() {
        splay();
        auto curr = this;
        while (curr->right) curr = curr->right;
        curr->splay();
        return curr;
    }

    bool am_i_left_son() {
        if (parent == 0 || parent->left == this) return 1;
        return 0;
    }

    /// @brief Update the size of the node's subtree and the subtree tag
    /// @details The subtree tag is the smallest tag in the subtree of the node
    void pull() {

        size = 1;
        if (left) size += left->size;
        if (right) size += right->size;

        subtree_tag = std::nullopt;

        if (left) {
            auto left_tag = left->subtree_tag;
            if (left_tag.has_value() &&
                (!subtree_tag.has_value() || left_tag->value < subtree_tag->value)) {
                subtree_tag = left_tag;
            }
        }

        if (right) {
            auto right_tag = right->subtree_tag;
            if (right_tag.has_value() &&
                (!subtree_tag.has_value() || right_tag->value < subtree_tag->value)) {
                subtree_tag = right_tag;
            }
        }

        if (!tags.empty() &&
            (!subtree_tag.has_value() || *tags.begin() < subtree_tag->value)) {
            subtree_tag = TagReference{this, *tags.begin()};
        }

    }

    void rotate_right() {
        auto& other = *left;

        if (parent) {
            if (am_i_left_son()) parent->left = &other;
            else parent->right = &other;
        }
        other.parent = parent;

        left = other.right;
        if (left) left->parent = this;

        parent = &other;
        other.right = this;

        pull();
        other.pull();
        parent->pull();
    }

    void rotate_left() {
        auto& other = *right;

        if (parent) {
            if (am_i_left_son()) parent->left = &other;
            else parent->right = &other;
        }
        other.parent = parent;

        right = other.left;
        if (right) right->parent = this;

        parent = &other;
        other.left = this;

        pull();
        other.pull();
        parent->pull();
    }

    /// @brief Rotate the node up one level
    void rotate_up() {
        auto& pr = *parent;
        if (am_i_left_son()) {
            pr.rotate_right();
        } else {
            pr.rotate_left();
        }
    }

    /// @brief Rotate the node upwards twice
    void double_rotation() {
        auto& pr = *parent;

        if (am_i_left_son() == pr.am_i_left_son()) {
            parent->rotate_up();
            rotate_up();
        } else {
            rotate_up();
            rotate_up();
        }
    }

    void splay_step() {
        auto& pr = *parent;
        if (pr.parent == 0) {
            // single rotation
            rotate_up();
        } else {
            // double rotation
            double_rotation();
        }
    }

    /// @brief Splay the node to the root
    void splay() {
        while (parent != 0) {
            splay_step();
        }
        pull();
    }

    SplayNode(DataType value) {
        left = right = parent = 0;
        data = value;
        size=1;
    }

    void add_tag(TagType tg) {
        tags.insert(tg);
        splay();
    }

    /// @brief Cut the left subtree of the node
    /// @return Pointer to the root of the cut subtree
    SplayNode* cut_left() {
        splay();
        if(left==0) return 0;
        SplayNode* res = left;
        left->parent = 0;
        left = 0;
        splay();
        return res;
    }

    /// @brief Cut the right subtree of the node
    /// @return Pointer to the root of the cut subtree
    SplayNode* cut_right() {
        splay();
        if(right==0) return 0;
        SplayNode* res = right;
        right->parent = 0;
        right = 0;
        splay();
        return res;
    }

    /// @brief Merge another subtree to the right of the current
    /// @param other The subtree to merge
    void merge_right(SplayNode* other) {
        if(other==0) return;
        auto rightmost = get_rightmost();
        other->splay();
        rightmost->right = other;
        other->parent = rightmost;
        rightmost->splay();
    }
};

}  // namespace dnc
