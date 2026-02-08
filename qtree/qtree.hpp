#pragma once

#include <vector>

#include "types/Point.hpp"
#include "types/Segment.hpp"
#include "types/Rock.hpp"


class Qtree {
private:
    class Colors;
    class Box;

public:
    enum Type { Undefined, Free, Mix, Busy, Inner };

private:
    struct Box {
        double x_min{0}, x_mid{0}, x_max{0}, y_min{0}, y_mid{0}, y_max{0};
        bool is_leaf{true};
        Type type{Type::Undefined};
        Box *q1{nullptr}, *q2{nullptr}, *q3{nullptr}, *q4{nullptr};

        std::vector<const Segment*> segments;
        std::vector<Rock> rocks;

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
    double min_box_size_point = 0.5;

public:
    Qtree() = default;
    Qtree(const Point& min, const Point& max) : root{create_box(min, max)} {}

    Qtree(const Qtree& other) : root(copy_box(other.root)), min_box_size_point(other.min_box_size_point) {}

    Qtree& operator=(const Qtree& other);

    ~Qtree() { clear(); }

public:
    bool collision(const Point& p);
    void add(const Segment& s);

private:
    Box* locate(const Point& p, Box* from);
    Box* locate(const Point& p);

    bool is_nearby(const Box* b1, const Box* b2);

    void expand(Box* box);
    Box* upscale(Box* b, const Point& p);

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


#include "add.inl"
#include "box.inl"
#include "collision.inl"
#include "expand.inl"
#include "is_inside.inl"
#include "is_nearby.inl"
#include "locate.inl"
#include "ostream.inl"
#include "qtree.inl"
#include "upscale.inl"
