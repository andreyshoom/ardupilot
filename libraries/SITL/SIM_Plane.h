/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/*
  simple plane simulator class
*/

#pragma once

#include "SIM_Aircraft.h"
#include "SIM_ICEngine.h"
#include <Filter/LowPassFilter.h>

namespace SITL {

/*
  a very simple plane simulator
 */
class Plane : public Aircraft {
public:
    Plane(const char *frame_str);

    /* update model by one time step */
    virtual void update(const struct sitl_input &input) override;

    /* static object creator */
    static Aircraft *create(const char *frame_str) {
        return NEW_NOTHROW Plane(frame_str);
    }

protected:
    const float hover_throttle = 0.65f;
    float angle_of_attack;
    float beta;

    struct {
        // from last_letter skywalker_2013/aerodynamics.yaml
        // thanks to Georacer!
        float s = 0.38;
        float b = 1.28;
        float c = 0.29;
        float c_lift_0 = 0.55;
        float c_lift_deltae = 0.1;
        float c_lift_a = 5.5;
        float c_lift_q = 2.0;
        float mcoeff = 40;
        float oswald = 0.8;
        float alpha_stall = 0.3491;
        float c_drag_q = 0.02;
        float c_drag_deltae = 0.02;
        float c_drag_p = 0.06;
        float c_y_0 = 0;
        float c_y_b = -0.85;
        float c_y_p = 0.15;
        float c_y_r = 0.3;
        float c_y_deltaa = 0.1;
        float c_y_deltar = -0.25;
        float c_l_0 = 0;
        float c_l_p = -0.9;
        float c_l_b = -0.15;
        float c_l_r = 0.1;
        float c_l_deltaa = 0.3;
        float c_l_deltar = -0.05;
        float c_m_0 = 0.02;
        float c_m_a = -0.5;
        float c_m_q = -10;
        float c_m_deltae = 0.8;
        float c_n_0 = 0;
        float c_n_b = 0.12;
        float c_n_p = 0.02;
        float c_n_r = -0.8;
        float c_n_deltaa = 0.02;
        float c_n_deltar = 0.2;
        float deltaa_max = 0.4363;
        float deltae_max = 0.4363;
        float deltar_max = 0.4363;
        // the X CoG offset should be -0.02, but that makes the plane too tail heavy
        // in manual flight. Adjusted to -0.15 gives reasonable flight
        Vector3f CGOffset{-0.1, 0, -0.03};
    } coefficient;

    float thrust_scale;
    bool reverse_thrust;
    bool elevons;
    bool vtail;
    bool dspoilers;
    bool redundant;
    bool reverse_elevator_rudder;
    bool ice_engine;
    bool tailsitter;
    bool aerobatic;
    bool copter_tailsitter;
    bool have_launcher;
    bool have_steering;
    float launch_accel;
    float launch_time;
    uint64_t launch_start_ms;

    const uint8_t throttle_servo = 2;
    const int8_t choke_servo = 14;
    const int8_t ignition_servo = 12;
    const int8_t starter_servo = 13;
    const float slewrate = 80;
    ICEngine icengine{
        throttle_servo,
        choke_servo,
        ignition_servo,
        starter_servo,
        slewrate,
        true
    };

    float liftCoeff(float alpha) const;
    float dragCoeff(float alpha) const;
    Vector3f getForce(float inputAileron, float inputElevator, float inputRudder) const;
    Vector3f getTorque(float inputAileron, float inputElevator, float inputRudder, float inputThrust, const Vector3f &force) const;
    void calculate_forces(const struct sitl_input &input, Vector3f &rot_accel);
};

} // namespace SITL
