#ifndef COLLIDER_HH_
# define COLLIDER_HH_

#include "System/ASystem.hh"

class ColliderSystem : public ASystem
{
    public:
        ColliderSystem() {}
        ColliderSystem(std::list<Entity*> *list) :
            _isActiv(false), _eList(list) {
                _eventList.push_back(E_Stage);
            }
        virtual ~ColliderSystem() {}


        void                            getSize(std::pair<int, int> &s,
                const std::string &type, const std::string &name)
        {
            if (type == "player")
            {
                s.first = 111;
                s.second = 39;
            }
            else if (type == "shoot")
            {
                s.first = 20;
                s.second = 12;
            }
            else if (type == "mob")
            {
                if (name == "mob-1")
                {
                    s.first = 97;
                    s.second = 72;
                }
                else if (name == "mob-2")
                {
                    s.first = 114;
                    s.second = 39;
                }
                else if (name == "mob-3")
                {
                    s.first = 58;
                    s.second = 87;
                }
            }
        }

        virtual void                    update(int)
        {
            std::pair<float, float> p1;
            std::pair<float, float> p2;
            std::pair<int, int>     s1;
            std::pair<int, int>     s2;
            std::string             t1;
            std::string             t2;

            if (_isActiv)
            {
                for (auto a : *_eList)
                {
                    std::cout << "FIRST " << std::endl;
                    p1 = a->manager.get<std::pair<float, float> >("position");
                    t1 = a->manager.get<std::string>("type");
                    getSize(s1, t1, a->manager.get<std::string>("name"));
                    for (auto b : *_eList)
                    {
                        if (a != b)
                        {
                            std::cout << "SECONd " << t1 << " " << t2 << std::endl;
                            p2 = b->manager.get<std::pair<float, float> >("position");
                            t2 = b->manager.get<std::string>("type");
                            getSize(s2, t2, b->manager.get<std::string>("name"));
                            if (t1 != t2 && p1.first < p2.first + s2.first &&
                                    p1.first + s1.first > p2.first &&
                                    p1.second < p2.second + s2.second &&
                                    s1.second + p1.second > p2.second) {
                                std::cout << "Allah u akbar" << std::endl;
                            }
                        }

                    }
                }
            }
        }
        virtual IPacket                 *out(EventSum &) { return 0; }
        virtual void                    in(IPacket*) {}

        virtual bool                    handle(EventSum e) {
            if (e == E_Stage)
                _isActiv = !_isActiv;
            return false;
        }

        virtual std::vector<REvent>     &broadcast(void) { return _eventList; }

        virtual EventSum                getEvent() { return noEvent; }
    private:
        bool	                        _isActiv;
        std::list<Entity*>              *_eList;

};

#endif
