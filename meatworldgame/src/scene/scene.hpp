#pragma once


namespace meat
{
    class Scene;
}



class meat::Scene
{
public:
    struct Node
    {
        virtual void update() {  };
    };

private:
    Node *m_root;

public:

    Scene()
    :   m_root(new Node)
    {
        
    }

};

