/*
 * alt_speed.c
 *
 *  Created on: Aug 5, 2024
 *      Author: kjkim
 */
#include "../../../Application/include/utils/alt_speed.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

double delT = 0.01;		// 100Hz

typedef struct {
    double A[2][2];
    double H[2];
    double Q[2][2];
    double R;
    double x[2];
    double P[2][2];
    int firstRun;
} KalmanFilter;
KalmanFilter kf;

void initKalmanFilter(KalmanFilter *kf) {
    kf->A[0][0] = 1.0; kf->A[0][1] = delT;
    kf->A[1][0] = 0.0; kf->A[1][1] = 1.0;
    kf->H[0] = 1.0; kf->H[1] = 0.0;
    kf->Q[0][0] = 0.01; kf->Q[0][1] = 0.0;
    kf->Q[1][0] = 0.0; kf->Q[1][1] = 1.5;
    kf->R = 2.0;
    kf->x[0] = 0.0; kf->x[1] = 0.0;
    kf->P[0][0] = 10.0; kf->P[0][1] = 0.0;
    kf->P[1][0] = 0.0; kf->P[1][1] = 10.0;
    kf->firstRun = 1;
}


void kalmanFilterUpdate(KalmanFilter *kf, double z, double *pos, double *vel) {
	double xp[2], Pp[2][2], K[2], y, S;

    // Predict
    xp[0] = kf->A[0][0] * kf->x[0] + kf->A[0][1] * kf->x[1];
    xp[1] = kf->A[1][0] * kf->x[0] + kf->A[1][1] * kf->x[1];
    Pp[0][0] = kf->A[0][0] * kf->P[0][0] + kf->A[0][1] * kf->P[1][0] + kf->Q[0][0];
    Pp[0][1] = kf->A[0][0] * kf->P[0][1] + kf->A[0][1] * kf->P[1][1] + kf->Q[0][1];
    Pp[1][0] = kf->A[1][0] * kf->P[0][0] + kf->A[1][1] * kf->P[1][0] + kf->Q[1][0];
    Pp[1][1] = kf->A[1][0] * kf->P[0][1] + kf->A[1][1] * kf->P[1][1] + kf->Q[1][1];

    // Update
    y = z - (kf->H[0] * xp[0] + kf->H[1] * xp[1]);
    S = Pp[0][0] + kf->R;
    K[0] = Pp[0][0] / S;
    K[1] = Pp[1][0] / S;

    kf->x[0] = xp[0] + K[0] * y;
    kf->x[1] = xp[1] + K[1] * y;
    kf->P[0][0] = Pp[0][0] - K[0] * Pp[0][0];
    kf->P[0][1] = Pp[0][1] - K[0] * Pp[0][1];
    kf->P[1][0] = Pp[1][0] - K[1] * Pp[0][0];
    kf->P[1][1] = Pp[1][1] - K[1] * Pp[0][1];

    *pos = kf->x[0];
    *vel = kf->x[1];
}

void KalmanInit() {
    initKalmanFilter(&kf);
}

float getVSpeedKalman(float P_meas, float deltaTime) {
	double pos, vel;

	delT = (double)(deltaTime / 1000.0);
	kf.A[0][1] = delT;

	kalmanFilterUpdate(&kf, P_meas, &pos, &vel);

	return (float)(pos);
}
