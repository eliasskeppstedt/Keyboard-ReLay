mac_cc = clang -g
mac_cflags = -Wall -Wextra
mac_frameworks = -framework ApplicationServices -framework CoreFoundation
mac_src = \
	src/main.c \
	src/relayEventHandler.c \
	src/initData.c \
	src/eventQueue.c \
	src/os/quartzEventService.c \
	lib/cJSON.c
out = ./out/keyboard_relay

lin_cc = gcc
lin_cflags = -Wall -Wextra
lin_src = \
	src/main.c \
	src/relayEventHandler.c \
	src/initData.c \
	src/eventQueue.c \
	src/os/x11EventSystem.c \
	lib/cJSON.c

macapp: $(mac_src)
	$(mac_cc) $(mac_cflags) $(mac_src) $(mac_frameworks) -o $(out)

linapp: $(lin_src)
	$(lin_cc) $(lin_cflags) $(lin_src) -o $(out)

run:
	$(out)

clean:
	rm $(out)
