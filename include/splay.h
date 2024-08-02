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

    void pop_tag(TagType tg) {
        auto ptr = tags.find(tg);
        if(ptr!=tags.end()) tags.erase(ptr);
        splay();
    }

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

    void pull() {

        size = 1;
        if (left) size += left->size;
        if (right) size += right->size;
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

        subtree_tag = std::nullopt;
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

    void rotate_up() {
        auto& pr = *parent;
        if (am_i_left_son()) {
            pr.rotate_right();
        } else {
            pr.rotate_left();
        }
    }

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

    SplayNode* cut_left() {
        splay();
        if(left==0) return 0;
        SplayNode* res = left;
        left->parent = 0;
        left = 0;
        splay();
        return res;
    }

    SplayNode* cut_right() {
        splay();
        if(right==0) return 0;
        SplayNode* res = right;
        right->parent = 0;
        right = 0;
        splay();
        return res;
    }

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
