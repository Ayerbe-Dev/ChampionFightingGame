#pragma once
void wait_ms(double ms_duration = (1000.0 / 60.0), bool process_time = true);
double get_ms_since_last_frame();