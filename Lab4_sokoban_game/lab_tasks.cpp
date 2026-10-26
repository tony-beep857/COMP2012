#include "GameObject.hpp"
#include "GameBoard.hpp"
#include "objects.hpp"

// Complete your code down below!
Player::Player(int x, int y) : GameObject(x, y, " P ", 6, true, true){}

Wall::Wall(int x, int y) : GameObject(x, y, "###", 5, true, false){}

Box::Box(int x, int y) : GameObject(x, y, " X ", 4, true, true){}

Door::Door(int x, int y) : GameObject(x, y, "[#]", 3, true, false){}

Button::Button(int x, int y, Door* door) : GameObject(x, y, "---", 2, false, false){
    this->door = door;
}

Flag::Flag(int x, int y) : GameObject(x, y, "!!!", 1, false, false){}

GameObject* GameBoard::find_collidable_object_at(int x, int y) const{
    for(int i = 0; i < object_count; i++){
        if(objects[i]->get_x() == x && objects[i]->get_y() == y && objects[i]->get_is_collidable() == true){
            return objects[i];
        }
    }
    return nullptr;
}

bool GameObject::try_move(GameBoard& board, Direction direction){
    if(!this->get_is_movable()){
        return false;
    }

    int new_x = this->get_x();
    int new_y = this->get_y();

    if (direction == NORTH){
        new_y -= 1;
    } else if (direction == SOUTH){
        new_y += 1;
    } else if (direction == EAST){
        new_x += 1;
    } else if (direction == WEST){
        new_x -= 1;
    }

    // find the new target element
    GameObject* object = board.find_collidable_object_at(new_x, new_y);
    if (object == nullptr){
        this->set_x(new_x);
        this->set_y(new_y);
        return true;
    } else {
        if (object->try_move(board, direction) == true){
            this->set_x(new_x);
            this->set_y(new_y);
            return true; 
        } else {
            return false;
        }
    }
}
void Door::open_door(){
    this->set_sprite("[ ]");
    this->set_is_collidable(false);
}

void Door::close_door(const GameBoard& board){

    GameObject* object = board.find_collidable_object_at(this->get_x(),this->get_y());
    if (object == nullptr) {//the object is not collidable
        this->set_sprite("[#]");
        this->set_is_collidable(true);
    }
}

void Button::update_button(const GameBoard& board){
    GameObject* object = board.find_collidable_object_at(this->get_x(), this->get_y());
    if (object == nullptr) {//no object is standing on the button 
        this->door->close_door(board);
        this->set_sprite("---");
    } else{
        this->door->open_door();
        this->set_sprite("___");
    }
}