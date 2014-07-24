CC = $(CROSS_COMPILE)gcc

TARGET_1 = endian$(EXE_SUFFIX)
OBJECTS_1 = endian.o

TARGET_2 = unbin$(EXE_SUFFIX)
OBJECTS_2 = unbin.o

TARGET_3 = mergebin$(EXE_SUFFIX)
OBJECTS_3 = mergebin.o

TARGET_4 = c02$(EXE_SUFFIX)
OBJECTS_4 = c02.o

all:$(TARGET_1) $(TARGET_2) $(TARGET_3) $(TARGET_4)

$(TARGET_1):$(OBJECTS_1)
	$(CC) $(OBJECTS_1) -o $(TARGET_1)
	
$(TARGET_2):$(OBJECTS_2)
	$(CC) $(OBJECTS_2) -o $(TARGET_2)
	
$(TARGET_3):$(OBJECTS_3)
	$(CC) $(OBJECTS_3) -o $(TARGET_3)
	
$(TARGET_4):$(OBJECTS_4)
	$(CC) $(OBJECTS_4) -o $(TARGET_4)
	
clean:
	rm -f $(OBJECTS_1)
	rm -f $(TARGET_1)
	rm -f $(OBJECTS_2)
	rm -f $(TARGET_2)
	rm -f $(OBJECTS_3)
	rm -f $(TARGET_3)
	rm -f $(OBJECTS_4)
	rm -f $(TARGET_4)