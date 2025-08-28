mac_cc = clang
mac_cflags = -Wall -Wextra
mac_frameworks = -framework ApplicationServices -framework CoreFoundation
mac_src = ./src/mac/quartzEventService.c
out = keyboard_relay

macapp: $(mac_src)
	$(mac_cc) $(mac_cflags) $(mac_src) $(mac_frameworks) -o $(out)

run:
	./$(out)

clean:
	rm $(out)