// Convert LED at (row, col) to corresponding LED index
int get_led_num(int row, int col);

// Set the RGB value for some given LED in the matrix
// led_num must be less than DATA_SIZE 
void set_data(int led_num, unsigned char r, unsigned char g, unsigned char b);

// Initializes the PWM driver
void pwm_init(void);

// Send PWM signals to LED matrix
void play_pwm();

// Print LEDs being lit up on the matrix
void debug_matrix();
