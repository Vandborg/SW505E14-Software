#ifndef BOOT_PROGRAMS_H_   /* Header guard */
    #define BOOT_PROGRAMS_H_

    #include "boot_programs/color_calibration/color_calibration.h"
    #include "boot_programs/color_sensor_measurement/color_sensor_measurement.h"
    #include "boot_programs/distance_measurement/distance_measurement.h"
    #include "boot_programs/fork_reset/fork_reset.h"
    #include "boot_programs/sonar_sensor_measurement/sonar_sensor_measurement.h"

    /*
     * Add a boot program
     * name     Name of the boot program
     * func     Function pointer to the boot program
     */
    void add_boot_program(char* name, void (*func)(void));

    /*
     * Check if a not default mode is wanted
     * name     Name of the boot program
     * func     Function pointer to the boot program
     */
    void boot_device(void);

#endif
