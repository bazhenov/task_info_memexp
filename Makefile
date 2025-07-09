main: main.c
	gcc -o $@ main.c

memexp: memexp.c
	gcc -O3 -o $@ $<

xpc: xpc.c
	gcc -o $@ $<
	#codesign --sign - --entitlements sysmond-client.plist --force --deep $@

sysmon: sysmon.c
	gcc -g -lsysmon -o $@ $<
	codesign --sign - --entitlements sysmond-client.plist --force --deep $@

clean:
	rm -f main
