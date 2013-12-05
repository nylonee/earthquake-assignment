/* Assignment 1 for Engineering Computation
 * Created by Nihal Mirpuri (nmirpuri@student.unimelb.edu.au) 25/04/2013
 * programming is fun
 *
 * Takes an input of earthquake data and outputs it in a readable format.
 */


/* Definitions and imports */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define IGNORE_LINE "%*[^\n]\n"
#define LOCATION_PRINT "(%.1f,%.1f)"
#define TIME_PRINT "%.4d-%.2d-%.2d, %.2d:%.2d:%.2d"
#define SEPARATOR "\n=======\n"
#define ENERGY_BAR "-"
#define ENERGY_SEPARATOR "+"
#define ENERGY_START "|"
#define ENERGY_FINISH "|"
#define MAXIMUM_LINES_INPUT 10000
#define ENERGY_FROM_MAGNITUDE(magnitude) pow(10, (3.0*(magnitude))/2.0 - 7.2)
#define GRAPH_FROM_ENERGY(energy) log10(energy/10)*10
#define ROUNDING_OFFSET 0.5
#define TIMESTAMP_VALUES 6
#define EARTHQUAKE_VALUES 4
#define RESET 0
#define TIME_YEAR 0
#define TIME_MONTH 1
#define TIME_DAY 2
#define TIME_HOUR 3
#define TIME_MINUTE 4
#define TIME_SECOND 5
#define MAG_LAT 0
#define MAG_LONG 1
#define MAG_DEPTH 2
#define MAG_MAG 3
#define POWER_INCREMENT 3


/* Function declarations */

int strongest_earthquake(double mag[MAXIMUM_LINES_INPUT][EARTHQUAKE_VALUES]);
double energy_calculate(double mag);
void energy_print(double energy);
void monthly_earthquake(int time[MAXIMUM_LINES_INPUT][TIMESTAMP_VALUES],
        double mag[MAXIMUM_LINES_INPUT][EARTHQUAKE_VALUES]);
void energy_print_graph(double energy);

int
main(int argc, char *argv[]) {

        int eCounter = 0, strongestEarthquake = 0, upperLimit = 0;
        double totalEnergy = 0;

        int (eTime[MAXIMUM_LINES_INPUT])[TIMESTAMP_VALUES];
        double (eMag[MAXIMUM_LINES_INPUT])[EARTHQUAKE_VALUES];


        /* Reads data from the source, ignoring the standard header
         * Data read is sorted into two arrays, one for time and one for data
         * eTime[][TIME_YEAR] = year of earthquake
         * (and so on)
         * ...
         * eMag[][MAG_LAT] = latitude of earthquake
         * eMag[][MAG_LONG] = longitude of earthquake
         * eMag[][MAG_DEPTH] = depth of earthquake
         * eMag[][MAG_MAG] = magnitude of earthquake
         */

        scanf(IGNORE_LINE);
        scanf(IGNORE_LINE);

        while(scanf("%4d-%2d-%2d%*c%2d:%2d:%2d+%*2d:%*2d,%lf,%lf,%lf,%lf",
                &eTime[eCounter][TIME_YEAR], &eTime[eCounter][TIME_MONTH],
                &eTime[eCounter][TIME_DAY], &eTime[eCounter][TIME_HOUR],
                &eTime[eCounter][TIME_MINUTE], &eTime[eCounter][TIME_SECOND],
                &eMag[eCounter][MAG_LAT], &eMag[eCounter][MAG_LONG],
                &eMag[eCounter][MAG_DEPTH], &eMag[eCounter][MAG_MAG])
                && eTime[eCounter][TIME_YEAR]!=0)
                eCounter++;

        if(eTime[0][TIME_YEAR] == 0) {
                printf("Error: No correct values entered!\n");
                exit(EXIT_FAILURE);
        }


        /* Stage 1 - Start Time
         * Lines will be read into the program and output:
         * a) The date, time, magnitude and location of the first earthquake
         * b) The energy released from according to the formula:
         *    E=(10^((3*magnitude)/2-7.2))
         */
        printf("\nStage 1"SEPARATOR);
        printf("Records commence: "TIME_PRINT"\n", eTime[0][TIME_YEAR],
                eTime[0][TIME_MONTH], eTime[0][TIME_DAY],
                eTime[0][TIME_HOUR], eTime[0][TIME_MINUTE],
                eTime[0][TIME_SECOND]);
        printf("First earthquake: magnitude %.1f at "LOCATION_PRINT"\n",
                eMag[0][MAG_MAG], eMag[0][MAG_LAT],
                eMag[0][MAG_LONG]);
        printf("Energy released : ");
        energy_print(energy_calculate(eMag[0][MAG_MAG]));


        /* Stage 3/4 - Monthly Reporting and Visualization
         * Lines will be read into the program and output:
         * a) The total earthquakes per month
         * b) The maximum magnitude earthquake per month
         * c) An energy bar of the total energy per month
         *
         * Note: Due to complexity the entirety of this stage will be done
         * in the function monthly_earthquake().
         */
        printf("\nStage 3/4"SEPARATOR);
        monthly_earthquake(eTime, eMag);


        /* Stage 2 - Overall Statistics
         * Lines will be read into the program and output:
         * a) The total earthquakes
         * b) The strongest earthquake's magnitude, location, date, and time
         * c) The total energy released
         * If there are two earthquakes with equal maximum magnitude,
         * the first one will be reported
         */
        strongestEarthquake = strongest_earthquake(eMag);
        printf("\nStage 2"SEPARATOR);
        printf("Total earthquakes    : %d\n", eCounter);
        printf("Strongest earthquake : magnitude %.1f at "LOCATION_PRINT"\n",
                eMag[strongestEarthquake][MAG_MAG],
                eMag[strongestEarthquake][MAG_LAT],
                eMag[strongestEarthquake][MAG_LONG]);
        printf("Took place on        : "TIME_PRINT"\n",
                eTime[strongestEarthquake][TIME_YEAR],
                eTime[strongestEarthquake][TIME_MONTH],
                eTime[strongestEarthquake][TIME_DAY],
                eTime[strongestEarthquake][TIME_HOUR],
                eTime[strongestEarthquake][TIME_MINUTE],
                eTime[strongestEarthquake][TIME_SECOND]);
        /* Finds the total energy of the month */
        for (upperLimit = 0; eMag[upperLimit][MAG_MAG] != 0;
                upperLimit++)
                totalEnergy = totalEnergy +
                        energy_calculate(eMag[upperLimit][MAG_MAG]);
        printf("Total energy released: ");
        energy_print(totalEnergy);

        return 0;
}

void
energy_print(double energy) {

        int powerIncrement = 0;

        while(energy > pow(10, POWER_INCREMENT)) {
                energy = energy/pow(10, POWER_INCREMENT);
                powerIncrement = powerIncrement + POWER_INCREMENT;
        }

        /* Prints earthquake energy with appropriate decimal points */
        if(energy < 10)
                printf("%.1f", energy);
        else
                printf("%.0f", energy);

        /* Prints the units of the earthquake in Tera, Peta or ExaJoules */
        switch(powerIncrement) {
        case 3:
                printf(" PJ\n");
                break;
        case 6:
                printf(" EJ\n");
                break;
        default:
                printf(" TJ\n");
                break;
        }
}

double
energy_calculate(double mag) {

        double energy = ENERGY_FROM_MAGNITUDE(mag);

        return energy;
}

void
monthly_earthquake(int time[MAXIMUM_LINES_INPUT][TIMESTAMP_VALUES],
        double mag[MAXIMUM_LINES_INPUT][EARTHQUAKE_VALUES]) {
        int currentYear = time[0][TIME_YEAR],
        currentMonth = time[0][TIME_MONTH];
        int currentEarthquake = 0, earthquakesInMonth = 1;
        double maxEarthquakeOfTheMonth = mag[0][MAG_MAG];
        double energyOfTheMonth = energy_calculate(mag[0][MAG_MAG]);

        printf("%.4d-%.2d: ", currentYear, currentMonth);

        /* Function loops until all months have been printed and
         * their corresponding data. Will break once the end of
         * the time[][] array has been reached.
         */
        while(1) {
                currentEarthquake++;

                if(time[currentEarthquake][TIME_YEAR] == currentYear &&
                        time[currentEarthquake][TIME_MONTH] == currentMonth)
                {

                        energyOfTheMonth = energyOfTheMonth +
                                energy_calculate(
                                        mag[currentEarthquake][MAG_MAG]);

                        if(mag[currentEarthquake][MAG_MAG] >
                                maxEarthquakeOfTheMonth)
                                maxEarthquakeOfTheMonth =
                                        mag[currentEarthquake][MAG_MAG];
                }
                else {
                        printf("%3d, max ", earthquakesInMonth);
                        printf("%.1f ", maxEarthquakeOfTheMonth);
                        energy_print_graph(energyOfTheMonth);

                        /* Will increment the month/year date until
                         * the program finds a month/year that there is
                         * existing earthquake data for.
                         */
                        do {
                                if(currentMonth == 12) {
                                        currentYear++;
                                        currentMonth = 1;
                                }
                                else
                                        currentMonth++;

                                if(time[currentEarthquake][TIME_YEAR]) {
                                        printf("%.4d-%.2d: ", currentYear,
                                                currentMonth);
                                        if(time[currentEarthquake][TIME_YEAR]
                                                != currentYear ||
                                                time[currentEarthquake]
                                                        [TIME_MONTH] !=
                                                currentMonth)
                                                printf("  0, max --- |\n");
                                }
                                else
                                        break;
                        }
                        while(time[currentEarthquake][TIME_YEAR]
                                != currentYear ||
                                time[currentEarthquake][TIME_MONTH]
                                != currentMonth);

                        earthquakesInMonth = RESET;
                        maxEarthquakeOfTheMonth =
                                mag[currentEarthquake][MAG_MAG];
                        energyOfTheMonth =
                                energy_calculate(mag[currentEarthquake]
                                        [MAG_MAG]);

                        if(!time[currentEarthquake][TIME_YEAR])
                                break;
                }
                earthquakesInMonth++;
        }
}

int
strongest_earthquake(double mag[MAXIMUM_LINES_INPUT][EARTHQUAKE_VALUES]) {

        int currentMax = 0, limit = 0;
        double magnitudeMax = 0.0;

        for (limit = 0; mag[limit][MAG_MAG] != 0; limit++) {
                if(mag[limit][MAG_MAG] > magnitudeMax) {
                        magnitudeMax = mag[limit][MAG_MAG];
                        currentMax = limit;
                }
        }
        return currentMax;
}


void
energy_print_graph(double energy) {

        /* Determines the length of the graph in bars, plus rounds it off */
        int graphLength = GRAPH_FROM_ENERGY(energy) + ROUNDING_OFFSET;
        int barCounter = 0;

        if(graphLength != 0 && energy != 0) {
                printf(ENERGY_START);

                while(graphLength != 1) {
                        barCounter++;

                        if(barCounter == 10) {
                                printf(ENERGY_SEPARATOR);
                                barCounter = RESET;
                        }
                        else
                                printf(ENERGY_BAR);
                        graphLength--;
                }
                printf(ENERGY_FINISH);
        }
        printf("\n");
}


