#ifndef REPLACEABLE_EDGE_HPP
#define REPLACEABLE_EDGE_HPP
typedef K::Segment_2 Segment_2;

struct Replaceable_edge{
  Point_2* potential_polygon_point;
  Segment_2 edge;
  std::size_t polygon_edge_index;
};

#endif
