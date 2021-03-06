#ifndef __NAVADATA__COMMON_H_
#define __NAVADATA__COMMON_H_

#include "typedef.h"
#include "quickmath.h"

typedef enum {
    GYRO_X      = 0,
    GYRO_Y      = 1,
    GYRO_Z      = 2,
    NB_GYRO     = 3
} def_gyro_t;

typedef enum {
    ACC_X       = 0,
    ACC_Y       = 1,
    ACC_Z       = 2,
    NB_ACCS     = 3
} def_acc_t;

typedef struct _velocities_t {
    float x;
    float y;
    float z;
} velocities_t;

enum navdata_tag_t : unsigned short
{
	NAVDATA_DEMO,
	NAVDATA_TIME,
	NAVDATA_RAW_MEASURES,
	NAVDATA_PHYS_MEASURES,
	NAVDATA_GYROS_OFFSETS,
	NAVDATA_EULER_ANGLES,
	NAVDATA_REFERENCES,
	NAVDATA_TRIMS,
	NAVDATA_RC_REFERENCES,
	NAVDATA_PWM,
	NAVDATA_ALTITUDE,
	NAVDATA_VISION_RAW,
	NAVDATA_VISION_OF,
	NAVDATA_VISION,
	NAVDATA_VISION_PERF,
	NAVDATA_TRACKERS_SEND,
	NAVDATA_VISION_DETECT,
	NAVDATA_WATCHDOG,
	NAVDATA_ADC_DATA_FRAME,
	NAVDATA_VIDEO_STREAM,
	NAVDATA_GAMES,
	NAVDATA_PRESSURE_RAW,
	NAVDATA_MAGNETO,
	NAVDATA_WIND,
	NAVDATA_KALMAN_PRESSURE,
	NAVDATA_HDVIDEO_STREAM,
	NAVDATA_WIFI,
	NAVDATA_ZIMMU_3000,
	NAVDATA_NUMS,
	NAVDATA_CKS = 0xffff,
};

#define NAVDATA_HEADER                  0x55667788

#define NAVDATA_MAX_SIZE                4096
#define NAVDATA_MAX_CUSTOM_TIME_SAVE    20


typedef struct _navdata_option_t {
    uint16_t    tag;   // Tag pour l'option spécifique
    uint16_t    size;  // Longeur de la structure

    uint8_t     data[];
} navdata_option_t;


typedef struct _navdata_t {
    uint32_t    header;
    uint32_t    ardrone_state;
    uint32_t    sequence;
    bool_t      vision_defined;

	navdata_option_t* options;
} navdata_t;

/**
 * @brief Minimal navigation data for all flights.
 */
typedef struct _navdata_demo_t {
  uint16_t    tag;					  /*!< Navdata block ('option') identifier */
  uint16_t    size;					  /*!< set this to the size of this structure */

  uint32_t    ctrl_state;             /*!< Flying state (landed, flying, hovering, etc.) defined in CTRL_STATES enum. */
  uint32_t    vbat_flying_percentage; /*!< battery voltage filtered (mV) */

  float   theta;                  /*!< UAV's pitch in milli-degrees */
  float   phi;                    /*!< UAV's roll  in milli-degrees */
  float   psi;                    /*!< UAV's yaw   in milli-degrees */

  int32_t     altitude;               /*!< UAV's altitude in centimeters */

  float   vx;                     /*!< UAV's estimated linear velocity */
  float   vy;                     /*!< UAV's estimated linear velocity */
  float   vz;                     /*!< UAV's estimated linear velocity */

  uint32_t    num_frames;			  /*!< streamed frame index */ // Not used -> To integrate in video stage.

  // Camera parameters compute by detection
  matrix33_t  detection_camera_rot;   /*!<  Deprecated ! Don't use ! */
  vector31_t  detection_camera_trans; /*!<  Deprecated ! Don't use ! */
  uint32_t	  detection_tag_index;    /*!<  Deprecated ! Don't use ! */

  uint32_t	  detection_camera_type;  /*!<  Type of tag searched in detection */

  // Camera parameters compute by drone
  matrix33_t  drone_camera_rot;		  /*!<  Deprecated ! Don't use ! */
  vector31_t  drone_camera_trans;	  /*!<  Deprecated ! Don't use ! */
} navdata_demo_t;



#endif