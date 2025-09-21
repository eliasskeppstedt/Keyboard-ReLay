mac_cc = clang
mac_cflags = -Wall -Wextra
mac_frameworks = -framework ApplicationServices -framework CoreFoundation
src = ./src/main.c \
	./src/converter.c \
	./src/os_specific/quartzEventService.c \
	./src/storeData.c \
	./src/keyHandler.c \
	./lib/cJSON.c
out = ./out/keyboard_relay

macapp: $(src)
	$(mac_cc) $(mac_cflags) $(src) $(mac_frameworks) -o $(out)

run:
	$(out)

clean:
	rm $(out)