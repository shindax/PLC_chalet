#define DS1307ADDR	0xD0

typedef struct {
	unsigned char second, minute, hour, day, date, month, year;
} date_time;

void read_time( date_time * time );
void write_time( date_time * time  );