    #include <stdio.h>
    #include <math.h>
    //#include "lcgrand.h"  /* Header file for random-number generator. */

    #define Q_LIMIT 100  /* Limit on queue length. */
    #define BUSY      1  /* Mnemonics for server's being busy */
    #define IDLE      0  /* and idle. */


    void  initialize(void);
    void  timing(void);
    void  arrive(void);
    void  depart(void);
    void  report(void);
    void  update_time_avg_stats(void);  
    float expon(float mean);
    float gen_next_interarrive(void);
    float  gen_next_depart(void);


    main()  /* Main function. */
    {

        /* Initialize the simulation. */

        initialize();

        /* Run the simulation while more delays are still needed. */
        
        while (final_condition) {

            /* Determine the next event. */
            
            timing();

            /* Update time-average statistical accumulators. */

            update_time_avg_stats();

            /* Invoke the appropriate event function. */

            switch (next_event_type) {
                case 1:
                    arrive();
                    break;
                case 2:
                    depart();
                    break;
            }
        }

        /* Invoke the report generator and end the simulation. */

        report();


        return 0;
    }


    void initialize(void)  /* Initialization function. */
    {
        /* Initialize the simulation clock. */

        sim_time = 0.0;

        /* Initialize the state variables. */

        server_status   = ...;
        num_in_q        = ...;
        time_last_event = ...;

        // Initialize the statistical counters. num_custs_delayed, total_of_delays, area_num_in_q, area_server_status

        ...

        /* Initialize event list.  Since no customers are present, the departure
        (service completion) event is eliminated from consideration. */

        time_next_event[1] = sim_time + gen_next_interarrive ; //Planificacion del primer arribo
        time_next_event[2] = 1.0e+30; //infinito
    }


    void timing(void)  /* Timing function. */
    {
        int   i;
        float min_time_next_event = 1.0e+29;

        next_event_type = 0; //Al final debe quedar en 1 en caso de que el proximo evento sea un arribo, 2: si es la salida de un servicio

        /* Determine the event type of the next event to occur. */

        ...


        /* advance the simulation clock. */

        ...
    }


    void arrive(void)  /* Arrival event function. */
    {
        float delay;

        /* Schedule next arrival. */

        time_next_event[1] = sim_time + ...;

        /* Check to see whether server is busy. */

        if (server_status == BUSY) {

            /* Server is busy, so increment number of customers in queue. */

        ...

            /* Check to see whether an overflow condition exists. */

            if (num_in_q > Q_LIMIT) {

                exit(2);
            }

            /* Guardar el tiempo de arribo de esta entidad para los calculos estadisticos */



        }

        else {

            /* Server libre, tener en  cuenta la entidad que pasa directamente al server para los calculos estadisticos */

        ...

            ...	


            /* Schedule a departure (service completion). */

            time_next_event[2] = sim_time + ...;
        }
    }


    void depart(void)  /* Departure event function. */
    {
        int   i;
        float delay;

        /* Check to see whether the queue is empty. */

        if (num_in_q == 0) {

            /* The queue is empty so make the server idle and eliminate the
            departure (service completion) event from consideration. */

            ... 
        }

        else {

            /* The queue is nonempty, so decrement the number of customers in
            queue. */

            ...

            /* Compute the delay of the customer who is beginning service and update
            the total delay accumulator. */

        ...

            /* Increment the number of customers delayed, and schedule departure. */

            ...
            time_next_event[2] = sim_time + ....;

        }
    }


    void report(void)  /* Report generator function. */
    {
        /* Compute and write estimates of desired measures of performance. */

        \\Average delay in queue

        \\Average number in queue

        \\Server utilization


    }


    void update_time_avg_stats(void)  /* Update area accumulators for time-average
                                        statistics. */
    {
        float time_since_last_event;

        /* Compute time since last event, and update last-event-time marker. */

        time_since_last_event = sim_time - time_last_event;
        time_last_event       = sim_time;

        /* Update area under number-in-queue function. */



        /* Update area under server-busy indicator function. */


    }


    float gen_next_interarrive()  
    {
    ...
    }

    float  gen_next_depart()
    {
    ...
    }
