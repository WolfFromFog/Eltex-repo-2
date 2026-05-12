CC = gcc
CFLAGS = -fPIC -Wall -Wextra
LDFLAGS = -ldl -export-dynamic

TARGET = calculator
PLUGIN_DIR = plugins
SOURCES = main.c calc.c
HEADERS = calc.h
OBJECTS = $(SOURCES:.c=.o)

PLUGIN_SOURCES = plugins/add.c plugins/div.c  #$(wildcard $(PLUGIN_DIR)/*.c)
PLUGIN_LIBS = $(PLUGIN_SOURCES:.c=.so)

all: plugins $(TARGET)

$(TARGET): $(OBJECTS) $(HEADERS)
	#$(CC) -o $@ $^ $(LDFLAGS)

plugins: $(PLUGIN_LIBS)

$(PLUGIN_DIR)/%.so: $(PLUGIN_DIR)/%.c plugin_interface.h
	$(CC) $(CFLAGS) -shared -o $@ $<

clean:
	rm -f $(OBJECTS) $(TARGET) $(PLUGIN_DIR)/*.so

run: all
	./$(TARGET)

.PHONY: all clean run plugins
