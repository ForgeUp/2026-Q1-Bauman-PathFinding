#pragma once

#include <list>
#include <set>
#include <map>

#include "types/Point.hpp"
#include "types/Segment.hpp"
#include "types/Rock.hpp"


class Qtree {
private:
    class Colors;
    class Box;

public:
    enum Type { Undefined, Free, Mix, Busy, Inner };

    using obstacle_t = Rock;
    using ObstID     = std::list<obstacle_t>::pointer;
    using SegIdx     = int32_t;

private:
    struct Box {
        double x_min{0}, x_mid{0}, x_max{0}, y_min{0}, y_mid{0}, y_max{0};
        bool is_leaf{true};
        Type type{Type::Undefined};
        Box *q1{nullptr}, *q2{nullptr}, *q3{nullptr}, *q4{nullptr};

        std::map<ObstID,std::set<SegIdx>> collisions;

        bool is_inside(const Point& p);
        bool is_inside_q1(const Point& p);
        bool is_inside_q2(const Point& p);
        bool is_inside_q3(const Point& p);
        bool is_inside_q4(const Point& p);
        double length();
        
        friend Qtree::Colors;
    };

    Box* create_box(const Point& min, const Point& max);
    Box* copy_box(const Box* other);
    void clear_box(Box* b);

    void clear();

private:
    Box* root{nullptr};
    Point min, max;
    
    std::list<obstacle_t> obstacles;

    double min_box_size_point = 0.5;

public:
    Qtree() = default;
    Qtree(const Point& min_, const Point& max_) : root{create_box(min_, max_)}, min{min_}, max{max_} {}

    Qtree(const Qtree& other) : root(copy_box(other.root)), min_box_size_point(other.min_box_size_point), min{other.min}, max{other.max} {}

    Qtree& operator=(const Qtree& other);

    ~Qtree() { clear(); }

public:
    bool collision(const Point& p);
    bool collision(const Segment& s);
    void add(const Rock& r);

private:
    Box* locate(const Point& p, Box* from);
    Box* locate(const Point& p);

    bool is_nearby(const Box* b1, const Box* b2);

    void expand(Box* box);
    Box* upscale(Box* b, const Point& p);

    void add(const Segment& s, SegIdx idx, ObstID r);

    template<bool upscale_on>
    std::vector<Qtree::Box*> trace(const Segment& s);

private:
    Segment get_segment_by_idx(ObstID obst, SegIdx idx);

public:
    friend std::ostream& operator<<(std::ostream& os, const Qtree& qtree);
    friend std::ostream& operator<<(std::ostream& os, const Colors& colors);
    
    Colors colors(Type select) const {
        return Colors(*this, select);
    }

    class Colors {
    private:
        const Qtree& qtree;
        Type select{Type::Undefined};
    public:
        Colors(const Qtree& qtree_, Type select_) : qtree(qtree_), select(select_) {}

        friend std::ostream& operator<<(std::ostream& os, const Colors& colors);
    };
};


#include "add_rock.inl"
#include "add_segment.inl"
#include "box.inl"
#include "collision.inl"
#include "expand.inl"
#include "is_inside.inl"
#include "is_nearby.inl"
#include "locate.inl"
#include "ostream.inl"
#include "qtree.inl"
#include "trace.inl"
#include "upscale.inl"
