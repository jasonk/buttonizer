#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

/*
 * These defines indicate which control lines the different items are
 * connected to.  You probably don't need to change this, as there aren't
 * a whole lot of options for which lines you can use anyway.
 */
#define RED_LED TIOCM_RTS
#define GREEN_LED TIOCM_DTR
#define SWITCH TIOCM_CAR

static int fd = 0;

/*
 * Called by signal handler, turn off all the LEDs and exit
 */
void cleanup ( int code ) {
    int state = 0;
    ioctl( fd, TIOCMSET, &state );
    exit( 0 );
}

/*
 * Returns true if the button is currently being pressed
 */
int button_pressed() {
    int serial = 0;
    if ( ioctl( fd, TIOCMGET, &serial ) < 0 ) {
        perror( "ioctl" );
        return -1;
    }

    return ( serial & SWITCH ) ? 1 : 0;
}

/*
 * Set the state of an LED
 *
 * The 'which' argument indicates which LED to change, call it with
 * either RED_LED or GREEN_LED.
 * The 'how' argument indicates how to change it.
 * 0 - Turn the LED off.
 * 1 - Turn the LED on.
 * -1 - Toggle the LED, turning it off if it's on and on if it's off.
 */
int setled ( int which, int how ) {
    int state = 0;
    if ( ioctl( fd, TIOCMGET, &state ) < 0 ) {
        perror( "ioctl TIOCMGET" );
        return -1;
    }
    // If how is negative then we just Toggle
    if ( how < 0 ) how = state & which;

    // http://graphics.stanford.edu/~seander/bithacks.html
    //  -> Conditionally set or clear bits without branching
    state ^= ( -how ^ state ) & which;

    if ( ioctl( fd, TIOCMSET, &state ) < 0 ) {
        perror( "ioctl TIOCMSET" );
        return -1;
    }
    return state;
}

void usage() {
    printf( "USAGE: buttonizer device command\n" );
    printf( "\n" );
    printf( "EXAMPLE:\n" );
    printf( "    buttonizer /dev/ttyS0 /usr/local/bin/panic-button\n" );
    exit( 1 ); 
}


int main ( int argc, char **argv ) {
    // pid_t pid;
    int res, status_led, blinks;

    // Must be called with 3 arguments
    if ( argc != 3 ) usage();

    if ( access( argv[2], F_OK|X_OK ) != 0 ) {
        fprintf( stderr, "Command does not exist or is not executable!" );
        exit( EXIT_FAILURE );
    }

    // Open the serial port
    if ( ( fd = open( argv[1], O_RDWR | O_NDELAY ) ) < 0 ) {
        fprintf( stderr, "Failed to open serial port" );
        perror( "open" );
        exit( EXIT_FAILURE );
    }

    // Register signal handlers to turn off the LEDs before exiting
    signal( SIGINT, cleanup );
    signal( SIGTERM, cleanup );

    // turn off the red led and turn on the green one
    // to indicate that we're working
    setled( GREEN_LED, 0 );
    setled( RED_LED, 1 );

    /*
    // fork into the background
    if ( ( pid = fork() ) < 0 ) {
        perror( "fork failed" );
        exit( 1 );
    } else if ( pid > 0 ) {
        exit( 0 );
    }
    */

    // daemonize
    while( 1 ) {
        if ( button_pressed() ) {
            setled( GREEN_LED, 0 );
            setled( RED_LED, 1 );
            res = system( argv[2] );
            status_led = ( res == 0 ) ? GREEN_LED : RED_LED;
            blinks = 30;
            setled( GREEN_LED, 0 );
            setled( RED_LED, 0 );
            while ( blinks > 0 ) {
                setled( status_led, -1 );
                sleep( 1 );
            }
        }
        sleep( 1 );
    }
    return ( 1 );
}
