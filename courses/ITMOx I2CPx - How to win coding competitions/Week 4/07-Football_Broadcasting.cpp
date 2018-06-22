/*
   Week 4, 7

   ITMOx: I2CPx How to win coding competitions: secrets of champions"
   Week 4 > 4th Week Problems > Football Broadcasting
   https://goo.gl/MQGe4A
   training:   https://goo.gl/3BvHAe

   04.11.2016  /  "You have used 1 of 200 attempts" / "Runtime error, test #2"
                  "You have used 4 of 200 attempts" / "Wrong answer, test #2"
                  "You have used 6 of 200 attempts" / "Presentation error, test #5"
                  "You have used 10 of 200 attempts" / "Presentation error, test #5"
                        not because of "solution_obvious"

   08.03.2017  /  Time limit exceeded, test 17
                  
      - reversed backtracking (only):   "Time limit exceeded, test #17"
      !!!   use the fact that some nodes were already visited (by frame) = valid map for each frame (initially only the visible frame)

   30.04.2017  /  "Correct solution!", 40 tests
                  - solution_backtracking still does not work:  
                        "Presentation error, test #5"
                        "Instead of a 32-bit signed integer number, your program printed 'Impossible'."

   \todo:
      - "Presentation error, test #5"
            solution_backtracking: why? what is the difference?
*/

#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <assert.h>
#include <vector>

struct Point
{
   int x, y;
   Point(): x(0), y(0) {}
   Point(unsigned x_, unsigned y_): x(x_), y(y_) {}

   void Set(unsigned x_, unsigned y_)
   {
      x = x_; y= y_;
   }

   bool Left() 
   {
      if (!x)
         return false;
      --x;
      return true;
   }
   bool Right(unsigned W) 
   {
      if (x == W-1)
         return false;
      ++x;
      return true;
   }
   bool Up() 
   {
      if (!y)
         return false;
      --y;
      return true;
   }
   bool Down(unsigned H) 
   {
      if (y == H-1)
         return false;
      ++y;
      return true;
   }

   bool Valid(unsigned W, unsigned H) const
   {
      return (0 <= x && 0 <= y && x < (int)W && y < (int)H); 
   }

   bool operator <= (const Point& other) const
   {
      return (x <= other.x && y <= other.y);
   }
   bool operator == (const Point& other) const
   {
      return (x == other.x && y == other.y);
   }
};

typedef std::pair<Point, Point> Frame;
typedef std::vector<Frame> Frames;
typedef std::vector<Point> PlayerPositions;

struct Geometry
{
   Geometry(unsigned frames, unsigned W, unsigned H): m_frames(frames), m_W(W), m_H(H)
   {
      assert(0 < m_frames);
      assert(0 < m_W);
      assert(0 < m_H);
   }
   const unsigned m_frames, m_W, m_H;
};

class PositionsMemory
{
public:
   PositionsMemory(unsigned frames, unsigned W, unsigned H): m_geometry(frames, W, H)
   {
      //m_positions = nullptr;
   }
   //~PositionsMemory()
   //{
   //   delete [] m_positions;
   //}
   void Init()
   {
      //m_positions = new bool[m_geometry.m_frames*m_geometry.m_H*m_geometry.m_W];
      //for (size_t i = 0; i < m_geometry.m_frames*m_geometry.m_H*m_geometry.m_W; ++i)
      //    m_positions[i] = true;
      m_positions.resize(m_geometry.m_frames*m_geometry.m_H*m_geometry.m_W, true);
   }
   bool IsGood(unsigned frame, const Point& pt) const
   {
      return IsGood(frame, pt.x, pt.y);
   }
   void SetWrong(unsigned frame, const Point& pt)
   {
      return SetWrong(frame, pt.x, pt.y);
   }
   const Geometry& geometry() const 
   {
      return m_geometry;
   };
private:
   bool IsInitialized() const
   {
      //return !!m_positions;
      return !m_positions.empty();
   }
   void SetWrong(unsigned frame, unsigned w, unsigned h)
   {
      if (!IsInitialized())
         return;

      assert(frame < m_geometry.m_frames);
      assert(w < m_geometry.m_W);
      assert(h < m_geometry.m_H);
      assert(IsGood(frame, w, h));

      m_positions[frame*m_geometry.m_H*m_geometry.m_W+w*m_geometry.m_H+h] = false;
   }
   bool IsGood(unsigned frame, unsigned w, unsigned h) const
   {
      if (!IsInitialized())
         return true;

      assert(frame < m_geometry.m_frames);
      assert(w < m_geometry.m_W);
      assert(h < m_geometry.m_H);

      return m_positions[frame*m_geometry.m_H*m_geometry.m_W+w*m_geometry.m_H+h];
   }
private:
   const Geometry m_geometry;

   //bool* m_positions;
   std::vector<bool> m_positions;
};

static
bool FindTwoPositions(const std::vector<std::vector<bool>>& occupied, Point& one, Point& two)
{
   const auto W = occupied.size();
   const auto H = occupied[0].size();

   for (unsigned i = 0; i < W; ++i)
      for (unsigned j = 0; j < H; ++j)
      {
         if (occupied[i][j])
            continue;
         if (i != W-1 && !occupied[i+1][j])
         {
            one.Set(i, j);
            two.Set(i+1, j);
            return true;
         }
         if (j != H-1 && !occupied[i][j+1])
         {
            one.Set(i, j);
            two.Set(i, j+1);
            return true;
         }
      }

   return false;
}

static
bool solution_obvious(unsigned W, unsigned H, const Frames& frames, PlayerPositions& player)
{
   std::vector<std::vector<bool>> occupied;
   occupied.resize(W);
   for (auto& column : occupied)
      column.resize(H);

   for (auto& frame : frames)
   {
      for (auto i = frame.first.x; i <= frame.second.x; ++i)
         for (auto j = frame.first.y; j <= frame.second.y; ++j)
            occupied[i][j] = true;
   }

   Point one, two;
   if (!FindTwoPositions(occupied, one, two))
      return false;
   assert(one.Valid(W, H));
   assert(two.Valid(W, H));

   for (size_t i = 0; i < frames.size(); ++i)
   {
      player[i] = i%2 ? one : two;
   }

   return true;
}

static inline
bool Inside(const Frame& frame, const Point& pt)
{
   return (pt.x >= frame.first.x && pt.x <= frame.second.x && pt.y >= frame.first.y && pt.y <= frame.second.y);
}

static inline
bool Adjacent(const Point& one, const Point& two)
{
   const Point& first = (one <= two) ? one : two;
   const Point& second = (one <= two) ? two : one;
   Point temp(first);
   ++temp.x;
   if (temp == second)
      return true;
   --temp.x;
   ++temp.y;
   return (temp == second);
}

static inline
bool next_valid_point(unsigned W, unsigned H, Point& next)
{
   ++next.x;
   if (W != next.x)
      return true;

   next.x = 0;
   ++next.y;
   return (H != next.y);
}

static inline
bool next_valid_point_base(unsigned W, unsigned H, const Frame& frame, Point& pt)
{
   assert(pt.Valid(W, H));

   if (!next_valid_point(W, H, pt))
      return false;
   if (!Inside(frame, pt))
      return true;
   // try next good point outside the frame
   if (frame.second.x != W-1)
   {
      pt.x = frame.second.x+1;
      return true;
   }
   if (frame.second.y != H-1)
   {
      pt.x = 0;
      pt.y = frame.second.y+1;
      return true;
   }
   return false;
}

static inline
bool NextValidPoint(unsigned W, unsigned H, const Frame& frame, Point& pt)
{
   if (!next_valid_point_base(W, H, frame, pt))
      return false;
   assert(pt.Valid(W, H));
   return true;
}

static inline
bool Next(unsigned W, unsigned H, const Frame& frame, const Point& prev, unsigned direction, Point& pt_out)
{
   assert(direction < 4);

   auto pt = prev;
   switch (direction)
   {
   case 0: 
      if (!pt.Left() || Inside(frame, pt)) return false; 
      break;
   case 1:
      if (!pt.Right(W) || Inside(frame, pt)) return false; 
      break;
   case 2: 
      if (!pt.Up() || Inside(frame, pt)) return false;
      break;
   case 3:
      if (!pt.Down(H) || Inside(frame, pt)) return false; 
      break;
   }

   pt_out = pt;
   assert(pt.Valid(W, H));

   return true;
}

namespace solution_backtracking
{

static
bool try_next_frame(unsigned W, unsigned H, const Frames& frames, unsigned position, /*frame number*/ PositionsMemory& memory, PlayerPositions& player)
{
   assert(0 < position && position < frames.size());

   auto& frame = frames[position];
   auto& current = player[position];

   for (unsigned direction = 0; direction < 4; ++direction)
   {
      if (!Next(W, H, frame, player[position-1], direction, current))
         continue;
      if (!memory.IsGood(position, current))
         continue;
      if (position+1 == frames.size())
         return true;
      if (try_next_frame(W, H, frames, position+1, memory, player))
         return true;

      memory.SetWrong(position, current);
   };

   return false;
}

static
bool try_all(unsigned W, unsigned H, const Frames& frames, PlayerPositions& player)
{
   auto& frame = frames[0];
   auto& current = player[0];

   if (Inside(frame, current) && !NextValidPoint(W, H, frame, current))
      return false;
   if (1 == frames.size())
      return true;

   PositionsMemory m_allowed(frames.size(), W, H);
   if (1)
      m_allowed.Init();

   do 
   {
      if (try_next_frame(W, H, frames, 1, m_allowed, player))
         return true;
   }
   while (NextValidPoint(W, H, frame, current));

   return false;
}

}
// namespace solution_backtracking

namespace solution_reversed_backtracking
{

static
bool try_next_frame(unsigned W, unsigned H, const Frames& frames, unsigned position /*frame number*/, PositionsMemory& memory, PlayerPositions& player)
{
   assert(0 <= position && position < frames.size()-1);

   auto& frame = frames[position];
   auto& current = player[position];

   // move from the current good position to the next one
   for (unsigned direction = 0; direction < 4; ++direction)
   {
      if (!Next(W, H, frame, player[position+1] /*previous*/, direction, current))
         continue;
      if (!memory.IsGood(position, current))
         continue;
      if (position == 0)
         return true;
      if (try_next_frame(W, H, frames, position-1, memory, player))
         return true;

      memory.SetWrong(position, current);
   };

   return false;
}

static
bool try_all(unsigned W, unsigned H, const Frames& frames, PlayerPositions& player)
{
   const auto level = frames.size()-1;

   auto& frame = frames[level];
   auto& current = player[level];

   if (Inside(frame, current) && !NextValidPoint(W, H, frame, current))
      return false;
   // one level only
   if (!level)
      return true;

   PositionsMemory m_allowed(frames.size(), W, H);
   // uncomment =>  "Time limit exceeded, test 17"
   if (1)
      m_allowed.Init();

   // there are not "already validated points" at the first level since we visit them only once
   do 
   {
      if (try_next_frame(W, H, frames, level-1, m_allowed, player))
         return true;
   }
   while (NextValidPoint(W, H, frame, current));

   return false;
}

}
// namespace solution_reversed_backtracking

static 
bool solution(unsigned W, unsigned H, const Frames& frames, PlayerPositions& player_out)
{
   PlayerPositions player;
   player.resize(frames.size());

   if (solution_obvious(W, H, frames, player))
   {
      player_out = player;
      return true;
   }

   if (0)
   if (solution_backtracking::try_all(W, H, frames, player))
   {
      player_out = player;
      return true;
   }

   if (1)
   if (solution_reversed_backtracking::try_all(W, H, frames, player))
   {
      player_out = player;
      return true;
   }

   return false;
}

static
void problem()
{
   FILE* in = fopen("broadcast.in", "r");
   assert(in);
   FILE* out = fopen("broadcast.out", "w");
   assert(out);

   unsigned W, H, N;
   auto res = fscanf(in, "%u %u %u", &W, &H, &N);
   assert(res == 3);

   Frames frames;
   frames.resize(N);
   for (auto& frame : frames)
   {
      res = fscanf(in, "%u %u %u %u", &frame.first.x, &frame.first.y, &frame.second.x, &frame.second.y);
      assert(res == 4);
      --frame.first.x; --frame.first.y; --frame.second.x; --frame.second.y;

      assert(frame.first.Valid(W, H) && frame.second.Valid(W, H));
      assert(frame.first <= frame.second);
   }

   PlayerPositions player;

   if (!solution(W, H, frames, player))
      fprintf(out, "Impossible");
   else
   {
      assert(player.size() == frames.size());
      for (size_t i = 0; i < player.size(); ++i)
      {
         const auto& pos = player[i];

         assert(pos.Valid(W, H));
         assert(!Inside(frames[i], pos));
         assert(!i || Adjacent(pos, player[i-1]));

         fprintf(out, "%u %u\n", 1+pos.x, 1+pos.y);
      }
   }

   res = fclose(out);
   assert(res == 0);
}

int main()
{
   problem();
   return 0;
}