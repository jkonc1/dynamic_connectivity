#include <queue>
#include <iostream>
#include <set>

template<class DataType, class TagType>
struct SplayNode{
    struct TagReference{
        SplayNode* node;
        TagType value;
    };

    SplayNode *left, *right;
    SplayNode* parent;
    std::multiset<TagType> tags;
    TagReference subtree_tag;
    DataType data;

    TagReference get_tag(){
        return subtree_tag;
    }

    bool has_tag(TagType tg){
        return tags.count(tg);
    }

    void remove_tag(TagType tg){
        splay();
        auto it = tags.find(tg);
        if(it != tags.end()) tags.erase(it);
        update_tag();
    }

    SplayNode* get_rightmost(){
        splay();
        auto curr=this;
        while(curr->right) curr=curr->right;
        curr->splay();
        return curr;
    }

    bool am_i_left_son(){
        if(parent == 0 || parent->left == this) return 1;
        return 0;
    }

    void update_tag(){
        if(left){
            auto left_tag = left->get_tag();
            if(left_tag.node != 0){
                subtree_tag = left_tag;
                return;
            }
        }

        if(right){
            auto right_tag = right->get_tag();
            if(right_tag.node != 0){
                subtree_tag = right_tag;
                return;
            }
        }

        if(!tags.empty()){
            subtree_tag = {this, *begin(tags)};
            return;
        }

        subtree_tag.node = 0;
    }

    void rotate_right(){
        auto& other = *left;

        if(parent){
            if(am_i_left_son()) parent->left = &other;
            else parent->right = &other;
        }
        other.parent = parent;
        parent = &other;

        left = other.right;
        if(left) left->parent = this;
        other.right = this;

        update_tag();
        other.update_tag();
    }
    void rotate_left(){
        auto& other = *right;
        
        if(parent){
            if(am_i_left_son()) parent->left = &other;
            else parent->right = &other;
        }
        other.parent = parent;
        parent = &other;

        right = other.left;
        if(right) right->parent = this;
        other.left = this;

        update_tag();
        other.update_tag();
    }

    void rotate_up(){
        auto& pr = *parent;
        if(am_i_left_son()){
            pr.rotate_right();
        }
        else{
            pr.rotate_left();
        }
    }

    void double_rotation(){
        auto& pr = *parent;

        if(am_i_left_son() == pr.am_i_left_son()){
            parent->rotate_up();
            rotate_up();
        }
        else{
            rotate_up();
            rotate_up();
        }
    }

    void splay_step(){
        auto& pr = *parent;
        if(pr.parent == 0){
            // single rotation
            rotate_up();
        }
        else{
            // double rotation
            double_rotation();
        }
    }

    void splay(){
        while(parent != 0){
            splay_step();
        }
    }

    SplayNode(DataType value){
        left=right=parent=0;
        data=value;
    }

    void add_tag(TagType tg){
        tags.insert(tg);
        splay();
    }
};
