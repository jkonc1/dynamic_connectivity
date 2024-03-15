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

    std::optional<TagReference> get_tag() { return subtree_tag; }

    bool has_tag(TagType tg) { return tags.count(tg); }

    void remove_tag(TagType tg) {
        splay();
        auto it = tags.find(tg);
        if (it != tags.end()) tags.erase(it);
        pull();
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
            auto left_tag = left->get_tag();
            if (left_tag.has_value()) {
                subtree_tag = left_tag;
                return;
            }
        }

        if (right) {
            auto right_tag = right->get_tag();
            if (right_tag.has_value()) {
                subtree_tag = right_tag;
                return;
            }
        }

        if (!tags.empty()) {
            subtree_tag = {this, *begin(tags)};
            return;
        }

        subtree_tag = std::nullopt;
    }

    void rotate_right() {
        auto& other = *left;

        if (parent) {
            if (am_i_left_son())
                parent->left = &other;
            else
                parent->right = &other;
        }
        other.parent = parent;

        left = other.right;
        if (left) left->parent = this;

        parent = &other;
        other.right = this;

        pull();
        other.pull();
    }
    void rotate_left() {
        auto& other = *right;

        if (parent) {
            if (am_i_left_son())
                parent->left = &other;
            else
                parent->right = &other;
        }
        other.parent = parent;

        right = other.left;
        if (right) right->parent = this;

        parent = &other;
        other.left = this;

        pull();
        other.pull();
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
        pull();
        return res;
    }

    SplayNode* cut_right() {
        splay();
        if(right==0) return 0;
        SplayNode* res = right;
        right->parent = 0;
        right = 0;
        pull();
        return res;
    }

    void attach_left(SplayNode* other) {
        splay();
        if(other==0) return;
        assert(left == 0);
        left = other;
        other->parent = this;
        pull();
    }

    void attach_right(SplayNode* other) {
        splay();
        if(other==0) return;
        assert(right==0);
        right = other;
        other->parent = this;
        pull();
    }
};

}  // namespace dnc
