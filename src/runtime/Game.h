#pragma once

class FunBox;

class Game {
public:
  explicit Game(const char* name);
  virtual ~Game() = default;

  const char* name() const;

  virtual void start(FunBox& box) = 0;
  virtual void update(FunBox& box) = 0;
  virtual void stop(FunBox& box) = 0;

private:
  const char* _name;
};