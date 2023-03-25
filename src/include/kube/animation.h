// Kube - A puzzle video game.
// Copyright (C) 2021 Kendal Harland GPL-3.0-only
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef _ANIMATION_H
#define _ANIMATION_H

#include <glm/glm.hpp>
#include <math.h> /* sin */

/*
 * How flutter does animations
 *
 * Flutter uses an `Animation` type to keep track of the animation state, which
 * is represented by the closed interval [0, 1] where 0 == not started and 1 ==
 * complete. The animation always moves forward, from 0 to 1.  The animation is
 * updated incrementally based on the amount of time that has passed
 * (deltaTime) which is assumed to be mapped from clock-time to wall time by
 * the caller. If we want the animation to complete faster, we can multiple
 * deltaTime by some constant.
 *
 * Tweens are used to map real-world values to the positions on the animation
 * interval. For example a tween from the color red to the color blue would
 * determine how much the red has been converted to blue. If the animation is
 * 50% complete, this might result in the color purple.  Tweens are the
 * "visible" portion of the animation. For example, if you want to move an
 * object from point A to point B, you'd specify a tween of (A, B) and a curve,
 * and internally the program would manage the animation and transformation.
 *
 * Curves are used to move along the range [0, 1]. They are expressed as
 * functions of a t value, which is basically the x-axis value between 0 and 1.
 * Curves determine *how* the animation moves along the Tween's range. For
 * example, a sin curve will oscillate between both endpoints. A exponential
 * curve will start slow, but increase in speed drastically with each step.  A
 * logrithmic curve will start fast, but taper off as it progresses, and
 * perhaps never complete depending on the limit of the curve.
 *
 * This is a useful way for us to animate our cube movement. To recap, our cube
 * has two possible movements:
 *
 * 1. Roll
 * 2. Slide
 *
 * The most important of these movements is Roll (Slide is not decidedly a part
 * of the) game yet. During cube movement, we apply transformations to the cube
 * as a whole, and let OpenGL handle transforming each of the cube's vertices,
 * so we just need to consider the animation from the perspective of the cube's
 * center. This is technically true for any model that we choose to animate.
 *
 * The cube's center does not take a straight path through space when the cube
 * rolls. Instead, it rotates about either the x or z axis, depending on the
 * roll direction. The axis is also off center because of collision with the
 * ground: It's origin is the bottom edge of the the cube's face that is being
 * rolled over. Simply put, the path is an arc.  An arc can be expressed as
 * many separate transformations that are simultaneously applied to the cube to
 * create the illusion of physics-based rolling.
 *
 * The transformations are:
 *
 * 1. The x coordinate,
 * 2. Either the y or z coordinate
 * 3. A 90 degree rotation.
 *
 *                  oo
 *   ooooo        ooooo        ooooo
 *   ooooo  ->  oooooooo  ->   ooooo
 *   ooooo       ooooo         ooooo
 *   ooooo        oo           ooooo
 *
 *
 * The x coordinate's transformation is continuous. It moves from one position
 * in the world, to another. The "curve" determines the rate of movement, which
 * in this case is parabolic.
 *
 * The y or z coordinate's transformation is a loop, it moves upward and
 * reaches its peak midway through the roll, then gradually returns to its
 * original value.
 *
 * The rotation is also continuous. The cube rotates a full 90 degrees during
 * the roll.
 *
 * These give us our tweens:
 * X: x1 -> x2
 * Y: y1 -> y2 (or Z)
 * R: r1 -> r1 + 90
 *
 * The X-value transformation needs a curve that pushes it from x1 to x2 with a
 * parabolic arc that reaches its minimum when the y-value transformation arch
 * reaches its maximum.
 *
 * The Y-value transformation needs a curve that pushes it from y1 to y2 at .5
 * then back to y1 at 1.
 */

namespace kube {

class DoubleTween {
public:
  DoubleTween() = delete;

  explicit DoubleTween(double a, double b) : _a(a), _b(b) {}

  double Compute(double t) { return (_b - _a) * t; }

private:
  double _a;
  double _b;
};

class Curve {
public:
  virtual double Value(double x) = 0;
};

class CurveFunction : public Curve {
public:
  typedef double (*Function)(double);

  explicit CurveFunction(Function f) : _f(f) {}

  double Value(double x) { return _f(x); }

private:
  Function _f;
};

const CurveFunction SinCurve = CurveFunction(&sin);
const CurveFunction LinearCurve = CurveFunction([](double t) { return t; });

class AnimationState {
public:
  bool IsComplete() { return _progress >= 1; }

  void Update(double delta) {
    _progress += delta;
    if (_progress > 1.f) {
      _progress = 1.f;
    }
  }

  double Progress() { return _progress; }

private:
  double _progress = 0;
};

class RotateAnimation {
public:
  RotateAnimation() = delete;

  explicit RotateAnimation(AnimationState state, DoubleTween tween, Curve &curve, glm::vec3 axis)
      : _animation(state), _tween(tween), _curve(curve), _axis(axis) {}

  double Update(double t) {
    _animation.Update(t);
    double position = _curve.Value(_animation.Progress());
    return _tween.Compute(position);
  }

  const glm::vec3 Axis() { return _axis; }

  bool IsComplete() { return _animation.IsComplete(); }

  double Progress() { return _animation.Progress(); }

private:
  DoubleTween _tween;
  AnimationState _animation;
  Curve &_curve;
  glm::vec3 _axis;
};

// TODO: TranslationAnimation
// TODO: RollAnimation which combines all of the above.

} // namespace kube

#endif // _ANIMATION_H