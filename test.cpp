//author: Stefan Caldararu

#include <vector>
#include <iostream>

class Obj
{
    public:
        int * doesntPersist;
        int doesPersist;
        Obj(){
            doesntPersist = new int;
        }
        ~Obj(){
            delete doesntPersist;
        }
};

int main(int argc, char ** argv)
{
    
    std::vector<Obj> objects;
    objects.reserve(5);
    for (int i = 0; i<5 ; i++){
        Obj temp;
        temp.doesPersist = 5;
        *temp.doesntPersist = 5;
        objects.push_back(temp);
    }

    //Does copy over
    std::cout << objects[0].doesPersist;
    //Doesn't copy over
    std::cout << objects[0].doesntPersist;
    return 0;
}