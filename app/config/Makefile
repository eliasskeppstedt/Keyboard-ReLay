mac_cc = clang
mac_cflags = -Wall -Wextra
mac_frameworks = -framework ApplicationServices -framework CoreFoundation
src = ./../src/constants.c ./../src/dataStorage.c ./../src/eventHandler.c ./../src/logic.c ./../src/main.c ./../src/quartzEventService.c ./../lib/cJSON.c
out = ./../out/keyboard_relay

macapp: $(src)
	$(mac_cc) $(mac_cflags) $(src) $(mac_frameworks) -o $(out)

run:
	$(out)

clean:
	rm $(out)