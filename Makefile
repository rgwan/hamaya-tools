CC = $(CROSS_COMPILE)gcc

TARGET_1 = endian$(EXE_SUFFIX)
OBJECTS_1 = endian.o

TARGET_2 = unbin$(EXE_SUFFIX)
OBJECTS_2 = unbin.o

all:$(TARGET_1) $(TARGET_2)

$(TARGET_1):$(OBJECTS_1)
	$(CC) $(OBJECTS_1) -o $(TARGET_1)
	
$(TARGET_2):$(OBJECTS_2)
	$(CC) $(OBJECTS_2) -o $(TARGET_2)
	
clean:
	rm -f $(OBJECTS_1)
	rm -f $(TARGET_1)
	rm -f $(OBJECTS_2)
	rm -f $(TARGET_2)