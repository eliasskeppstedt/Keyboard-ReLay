mac_cc = clang
mac_cflags = -Wall -Wextra
mac_frameworks = -framework ApplicationServices -framework CoreFoundation
src = ./src/main.c
out = keyboard_relay

macapp: $(src)
	$(mac_cc) $(mac_cflags) $(src) $(mac_frameworks) -o $(out)

run:
	./$(out)

clean:
	rm $(out)