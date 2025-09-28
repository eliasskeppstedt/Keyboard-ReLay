mac_cc = clang -g
mac_cflags = -Wall -Wextra
mac_frameworks = -framework ApplicationServices -framework CoreFoundation
src = ./src/main.c \
	./src/converter.c \
	./src/os_specific/quartzEventService.c \
	./src/storeData.c \
	./src/keyHandler.c \
	./src/eventQueue.c \
	./lib/cJSON.c
out = ./out/keyboard_relay

macapp: $(src)
	$(mac_cc) $(mac_cflags) $(src) $(mac_frameworks) -o $(out)

run:
	$(out)

clean:
	rm $(out)

debug:
	clang backtrace

test_simulatePress:
	$(mac_cc) $(mac_cflags) ./test/simulatePress.c $(mac_frameworks) -o ./test
