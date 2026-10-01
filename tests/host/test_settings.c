#include "test.h"
#include "settings.h"

static void test_defaults_and_valid(void) {
  Settings s;
  settings_defaults(&s);
  CHECK_EQ_INT(s.version, SETTINGS_VERSION);
  CHECK_EQ_INT(s.clock, -1);
  CHECK_EQ_INT(s.hour_color, HOUR_RED);
  CHECK(s.show_weather);
  CHECK(!s.fahrenheit);
  CHECK(s.animate);
  CHECK(s.vibe_disconnect);
  CHECK(s.tap_swap);
  CHECK_EQ_INT(s.low_battery, 20);
  CHECK(settings_valid(&s));
}

static void test_clock_setters(void) {
  Settings s;
  settings_defaults(&s);
  CHECK(settings_set_clock(&s, 0));
  CHECK_EQ_INT(s.clock, 0);
  CHECK(!settings_set_clock(&s, 0));
  CHECK(!settings_set_clock(&s, 2));
  CHECK_EQ_INT(s.clock, 0);

  settings_defaults(&s);
  CHECK(settings_set_clock_str(&s, "1"));
  CHECK_EQ_INT(s.clock, 1);
  CHECK(settings_set_clock_str(&s, "-1"));
  CHECK_EQ_INT(s.clock, -1);
  CHECK(!settings_set_clock_str(&s, "-1"));
  CHECK(!settings_set_clock_str(&s, "abc"));
  CHECK(!settings_set_clock_str(&s, ""));
  CHECK(!settings_set_clock_str(&s, "1x"));
  CHECK(!settings_set_clock_str(&s, NULL));
  CHECK_EQ_INT(s.clock, -1);
}

static void test_hour_color_setters(void) {
  Settings s;
  settings_defaults(&s);
  CHECK(settings_set_hour_color(&s, HOUR_GOLD));
  CHECK_EQ_INT(s.hour_color, HOUR_GOLD);
  CHECK(!settings_set_hour_color(&s, HOUR_GOLD));
  CHECK(!settings_set_hour_color(&s, 3));
  CHECK_EQ_INT(s.hour_color, HOUR_GOLD);

  settings_defaults(&s);
  CHECK(settings_set_hour_color_str(&s, "cream"));
  CHECK_EQ_INT(s.hour_color, HOUR_CREAM);
  CHECK(settings_set_hour_color_str(&s, "gold"));
  CHECK_EQ_INT(s.hour_color, HOUR_GOLD);
  CHECK(!settings_set_hour_color_str(&s, "gold"));
  CHECK(settings_set_hour_color_str(&s, "red"));
  CHECK_EQ_INT(s.hour_color, HOUR_RED);
  CHECK(settings_set_hour_color_str(&s, "2"));
  CHECK_EQ_INT(s.hour_color, HOUR_GOLD);
  CHECK(!settings_set_hour_color_str(&s, "blue"));
  CHECK(!settings_set_hour_color_str(&s, "Red"));
  CHECK_EQ_INT(s.hour_color, HOUR_GOLD);
}

static void test_low_battery_setters(void) {
  Settings s;
  settings_defaults(&s);
  CHECK(settings_set_low_battery(&s, 30));
  CHECK_EQ_INT(s.low_battery, 30);
  CHECK(!settings_set_low_battery(&s, 30));
  CHECK(!settings_set_low_battery(&s, 25));
  CHECK_EQ_INT(s.low_battery, 30);

  settings_defaults(&s);
  CHECK(settings_set_low_battery_str(&s, "30"));
  CHECK_EQ_INT(s.low_battery, 30);
  CHECK(!settings_set_low_battery_str(&s, "30"));
  CHECK(!settings_set_low_battery_str(&s, "37"));
  CHECK(!settings_set_low_battery_str(&s, "5"));
  CHECK_EQ_INT(s.low_battery, 30);
  CHECK(settings_set_low_battery_str(&s, "10"));
  CHECK_EQ_INT(s.low_battery, 10);
}

static void test_bool_setters(void) {
  Settings s;
  settings_defaults(&s);
  CHECK(settings_set_bool(&s.show_weather, 0));
  CHECK(!s.show_weather);
  CHECK(!settings_set_bool(&s.show_weather, 0));
  CHECK(settings_set_bool(&s.show_weather, 1));
  CHECK(s.show_weather);
  CHECK(!settings_set_bool(&s.show_weather, 42));
  CHECK(s.show_weather);
}

static void test_valid_rejects(void) {
  Settings s;
  settings_defaults(&s);
  s.version = 0;
  CHECK(!settings_valid(&s));
  settings_defaults(&s);
  s.clock = 2;
  CHECK(!settings_valid(&s));
  settings_defaults(&s);
  s.hour_color = 3;
  CHECK(!settings_valid(&s));
  settings_defaults(&s);
  s.low_battery = 25;
  CHECK(!settings_valid(&s));
}

int main(void) {
  test_defaults_and_valid();
  test_clock_setters();
  test_hour_color_setters();
  test_low_battery_setters();
  test_bool_setters();
  test_valid_rejects();
  TEST_MAIN_END();
}
