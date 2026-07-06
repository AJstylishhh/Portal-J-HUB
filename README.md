# Portal-J-HUB - Web Injection Test

This is a small test build, step one toward **Portal-J-HUB** — a
controller-friendly launcher for Xbox Cloud Gaming, GeForce NOW,
YouTube, Twitch, and other web services on Nintendo Switch.

This specific build does **not** have the full menu yet. It only
tests one thing: whether libnx's `JsExtension` feature actually lets
us inject custom JavaScript into a real external page (xbox.com/play)
when launched through the system browser applet.

That answer decides whether we can layer better-xcloud's enhancements
(proper mic handling, stats overlay, quality tweaks) on top of the
official Xbox Cloud Gaming site — which is the core idea behind
Portal-J-HUB's Xbox tile.

## What this build does

1. Launches showing a simple text menu
2. Press **A** to open the system browser at `https://www.xbox.com/play`
   with `JsExtension` enabled
3. Press **+** at the start to exit without launching

Right now the actual script-injection call is commented out in
`source/main.cpp` — this first version only confirms the browser
launches correctly with JsExtension turned on. Once that's confirmed
working on real hardware, the next version will add the actual
injection call and test whether a real script executes on the page.

## Building

This repo builds automatically via GitHub Actions on every push to
`main`.

1. Go to the **Actions** tab on GitHub
2. Open the latest run (or trigger one manually via "Run workflow")
3. Download the `nxportal-test-nro` artifact — it contains
   `nxportal-test.nro`

## Installing

Copy `nxportal-test.nro` to:

```
/switch/nxportal-test/nxportal-test.nro
```

on your SD card. It'll then appear in the Homebrew Menu (Album app).

## What to report back

After running this on-device:

- Did the system browser open at all when you pressed A?
- Did xbox.com/play load normally?
- Any crash, freeze, or error message shown on screen?

That feedback decides the next step: either digging into the real
injection call, or finding an alternate approach if JsExtension
doesn't behave the way we expect for external pages.
