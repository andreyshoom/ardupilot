#include "mode.h"
#include "Plane.h"

void ModeStabilize::update()
{
    plane.nav_roll_cd = 0;
    plane.nav_pitch_cd = 0;
}

void ModeStabilize::run()
{
//	dive_mode_enabled = true;
//	const char* message = "--------------------true---------------------";
//
//	    // Уровень важности сообщения: MAV_SEVERITY_INFO (информационное сообщение)
//	gcs().send_text(MAV_SEVERITY_INFO, "%s", message);
    plane.stabilize_roll();
    plane.stabilize_pitch();
    stabilize_stick_mixing_direct();
    plane.stabilize_yaw();

    output_pilot_throttle();
}
