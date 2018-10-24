#pragma once

#include <atomic>

#include <common/mavlink/offboard_control.hpp>
#include <common/messages/ctrl_params_t.hpp>
#include <gnc/control/pid.hpp>
#include <gnc/measurements/waypoint.hpp>
#include <gnc/state.hpp>

namespace maav
{
namespace gnc
{
enum ControlState
{
	CONTROL_STATE_STANDBY = 0,
	CONTROL_STATE_TAKEOFF,
	CONTROL_STATE_LAND,
	CONTROL_STATE_HOLD_ALT
};

class Controller
{
   public:
	Controller();
	~Controller();

	// TODO: create target struct
	void set_target(const Waypoint& waypoint);

	mavlink::InnerLoopSetpoint run(const State& state);

	void set_control_params(const ctrl_params_t& _params);

	ControlState get_control_state() const;
	bool set_control_state(const ControlState new_control_state);

   private:
	mavlink::InnerLoopSetpoint hold_altitude(const double altitude);

	// Ascend at a particular rate
	mavlink::InnerLoopSetpoint ascend_at_rate(const double rate);

	// Takeoff to takeoff_altitude at ascent_rate m/s
	mavlink::InnerLoopSetpoint takeoff(const double takeoff_altitude, const double ascent_rate = 1);

	// Land at current location at descent_rate m/s
	mavlink::InnerLoopSetpoint land(const double descent_rate = 1);

	ControlState current_control_state;

	State current_state;
	State previous_state;
	double dt;  // Difference in time between current and previous state

	float thrust_0 = 0.59;		  // Equilibrium Thrust (TODO: get real thrust data)
	double takeoff_altitude = 2;  // meters
	double takeoff_error = 0.25;
	control::Pid thrust_pid;
};

// global altitude for testing
// This needs to become "hold_altitude" a member variable in controller class
std::atomic<double> ALTITUDE = -0.1;

}  // namespace gnc
}  // namespace maav