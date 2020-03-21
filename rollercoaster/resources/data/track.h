#ifndef DATA_TRACK_H
#define DATA_TRACK_H

namespace data {

struct track_point {
  float x, y, z;
};

const track_point track[32] = {
    { 29.57f, -178.29f, 21.79f },
  { 192.40f, -122.40f, -39.63f },
  { 338.69f, -170.35f, -108.43f },
  { 412.94f, -177.20f, -110.91f },
  { 444.87f, -174.66f, -57.20f },
  { 404.46f, -183.49f, -1.71f },
  { 300.66f, -195.36f, 17.57f },
  { 114.37f, -147.64f, -127.17f },
  { -13.31f, -162.97f, -228.52f },
  { -109.69f, -165.54f, -253.77f },
  { -202.08f, -180.15f, -182.18f },
  { -297.03f, -173.19f, -171.22f },
  { -367.77f, -182.39f, -211.42f },
  { -378.71f, -181.33f, -298.69f },
  { -334.55f, -177.19f, -355.97f },
  { -281.80f, -188.18f, -458.92f },
  { -92.88f, -122.91f, -462.20f },
  { 71.82f, -179.21f, -344.05f },
  { 126.27f, -174.60f, -246.82f },
  { 79.95f, -171.97f, -184.68f },
  { -43.69f, -158.63f, -123.31f },
  { -124.37f, -195.40f, 3.35f },
  { -187.49f, -97.70f, 180.90f },
  { -163.69f, -146.15f, 291.91f },
  { -85.86f, -200.61f, 288.42f },
  { 30.62f, -183.21f, 266.64f },
  { 134.51f, -216.87f, 354.66f },
  { 42.70f, -210.88f, 427.35f },
  { -110.35f, -190.03f, 394.59f },
  { -221.77f, -188.89f, 288.94f },
  { -129.97f, -200.35f, 127.96f },
      { 29.57f, -178.29f, 21.79f }

/*  { 29.20f, -178.30f, 21.93f },
  { 68.32f, -177.49f, 7.25f },
  { 120.15f, -164.80f, -10.30f },
  { 151.41f, -149.25f, -21.58f },
  { 181.14f, -126.48f, -34.35f },
  { 257.93f, -109.76f, -71.29f },
  { 307.38f, -147.98f, -98.22f },
  { 349.10f, -175.20f, -110.48f },
  { 395.02f, -179.37f, -113.67f },
  { 427.99f, -173.99f, -103.28f },
  { 445.46f, -171.03f, -69.09f },
  { 431.84f, -187.06f, -24.16f },
  { 385.14f, -181.21f, 9.06f },
  { 333.38f, -187.24f, 26.40f },
  { 297.20f, -195.37f, 15.47f },
  { 202.53f, -185.70f, -53.51f },
  { 151.92f, -166.70f, -95.66f },
  { 118.23f, -148.92f, -124.10f },
  { 55.23f, -135.80f, -172.94f },
  { -16.16f, -164.02f, -230.58f },
  { -61.89f, -176.62f, -256.37f },
  { -103.01f, -175.39f, -255.72f },
  { -143.76f, -177.88f, -236.41f },
  { -178.00f, -184.06f, -206.93f },
  { -195.00f, -190.34f, -188.59f },
  { -224.21f, -186.58f, -169.40f },
  { -282.65f, -175.94f, -168.78f },
  { -327.73f, -167.86f, -180.37f },
  { -343.60f, -170.62f, -188.97f },
  { -356.40f, -177.70f, -199.42f },
  { -374.06f, -184.37f, -219.44f },
  { -379.69f, -182.49f, -235.54f },
  { -378.54f, -177.54f, -268.87f },
  { -378.14f, -181.34f, -299.91f },
  { -338.38f, -177.78f, -350.28f },
  { -305.03f, -171.70f, -405.96f },
  { -291.74f, -177.61f, -436.93f },
  { -280.07f, -189.16f, -461.18f },
  { -254.04f, -195.82f, -482.51f },
  { -228.06f, -193.81f, -489.56f },
  { -180.34f, -176.60f, -489.40f },
  { -136.69f, -152.14f, -480.75f },
  { -98.59f, -126.00f, -464.80f },
  { -47.95f, -103.35f, -440.63f },
  { -12.94f, -103.68f, -421.13f },
  { 20.60f, -120.45f, -396.98f },
  { 49.66f, -153.34f, -368.19f },
  { 71.20f, -178.66f, -344.77f },
  { 93.06f, -192.67f, -317.66f },
  { 107.52f, -189.75f, -298.21f },
  { 123.53f, -180.08f, -265.80f },
  { 124.94f, -169.71f, -228.15f },
  { 108.55f, -164.01f, -195.79f },
  { 80.47f, -171.86f, -184.86f },
  { 16.76f, -183.16f, -163.98f },
  { -35.29f, -161.45f, -130.73f },
  { -94.74f, -146.48f, -69.46f },
  { -113.39f, -171.42f, -29.80f },
  { -121.73f, -191.62f, -5.16f },
  { -129.65f, -197.89f, 20.81f },
  { -138.30f, -186.09f, 47.31f },
  { -166.77f, -123.48f, 120.80f },
  { -181.75f, -99.91f, 163.60f },
  { -190.89f, -100.80f, 201.41f },
  { -182.35f, -119.13f, 254.14f },
  { -169.05f, -135.94f, 283.44f },
  { -152.23f, -162.77f, 298.99f },
  { -107.93f, -194.43f, 294.32f },
  { -80.60f, -201.06f, 286.94f },
  { -45.31f, -200.72f, 278.07f },
  { 7.31f, -187.80f, 265.48f },
  { 109.20f, -174.42f, 284.61f },
  { 134.58f, -219.84f, 361.93f },
  { 107.07f, -213.14f, 406.99f },
  { 77.30f, -210.86f, 420.90f },
  { -22.57f, -208.26f, 428.48f },
  { -143.41f, -184.15f, 376.67f },
  { -212.06f, -187.67f, 317.81f },
  { -218.13f, -190.93f, 250.36f },
  { -175.27f, -198.80f, 168.44f },
  { -128.06f, -200.06f, 126.32f },
  { -76.79f, -182.72f, 82.26f },
  { -55.90f, -177.38f, 58.25f },
  { -24.64f, -175.51f, 40.82f },
  { 29.20f, -178.30f, 21.93f } */
};

} // namespace data 

#endif