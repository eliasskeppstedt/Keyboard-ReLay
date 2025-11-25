mac_cc = clang -g
mac_cflags = -Wall -Wextra
mac_frameworks = -framework ApplicationServices -framework CoreFoundation
src = src/main.c \
	src/relayEventHandler.c \
	src/initData.c \
	src/eventQueue.c \
	src/os/quartzEventService.c \
	lib/cJSON.c
out = ./out/keyboard_relay

macapp: $(src)
	$(mac_cc) $(mac_cflags) $(src) $(mac_frameworks) -o $(out)

run:
	$(out)

clean:
	rm $(out)
