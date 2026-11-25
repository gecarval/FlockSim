"""Generate the README diagrams: python3 docs/make_diagrams.py docs"""
import math
import os
import random
import sys

OUT = sys.argv[1]
os.makedirs(OUT, exist_ok=True)

BG = "#0f172a"
PANEL = "#1e293b"
GRID = "#334155"
TEXT = "#e2e8f0"
MUTED = "#94a3b8"
FOCUS = "#f59e0b"
NEIGH = "#38bdf8"
FORCE = "#f43f5e"
FOOD = "#4ade80"
FONT = "font-family='Segoe UI, Helvetica, Arial, sans-serif'"


def boid(x, y, ang, color, s=9.0, opacity=1.0):
    """Triangle pointing along ang (radians)."""
    pts = []
    for da, r in ((0, s), (2.5, s * 0.7), (-2.5, s * 0.7)):
        pts.append(f"{x + r * math.cos(ang + da):.1f},{y + r * math.sin(ang + da):.1f}")
    return f"<polygon points='{' '.join(pts)}' fill='{color}' fill-opacity='{opacity}'/>"


def arrow(x1, y1, x2, y2, color, w=3, mid="a"):
    return (f"<line x1='{x1:.1f}' y1='{y1:.1f}' x2='{x2:.1f}' y2='{y2:.1f}' "
            f"stroke='{color}' stroke-width='{w}' marker-end='url(#{mid})'/>")


def marker(mid, color):
    return (f"<marker id='{mid}' viewBox='0 0 10 10' refX='8' refY='5' markerWidth='5' "
            f"markerHeight='5' orient='auto-start-reverse'><path d='M0,0 L10,5 L0,10 z' "
            f"fill='{color}'/></marker>")


def svg(w, h, body, defs=""):
    return (f"<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 {w} {h}' width='{w}' "
            f"height='{h}' {FONT}><defs>{defs}</defs>"
            f"<rect width='{w}' height='{h}' rx='16' fill='{BG}'/>{body}</svg>\n")


def text(x, y, s, size=16, color=TEXT, weight="normal", anchor="middle"):
    return (f"<text x='{x}' y='{y}' font-size='{size}' fill='{color}' "
            f"font-weight='{weight}' text-anchor='{anchor}'>{s}</text>")


def save(name, content):
    with open(os.path.join(OUT, name), "w") as f:
        f.write(content)


# ---------------------------------------------------------------- 1. rules
def rules():
    W, H, PW = 960, 380, 300
    defs = marker("a", FORCE) + marker("v", MUTED)
    body = []
    panels = [
        ("Separation", "steer away from boids that are too close"),
        ("Alignment", "steer toward the neighbours' average heading"),
        ("Cohesion", "steer toward the neighbours' average position"),
    ]
    for i, (title, sub) in enumerate(panels):
        ox = 15 + i * (PW + 15)
        cx, cy = ox + PW / 2, 200
        body.append(f"<rect x='{ox}' y='15' width='{PW}' height='{H - 30}' rx='12' fill='{PANEL}'/>")
        body.append(text(cx, 48, title, 22, TEXT, "bold"))
        body.append(text(cx, 72, sub, 13, MUTED))
        body.append(f"<circle cx='{cx}' cy='{cy}' r='110' fill='none' stroke='{GRID}' "
                    f"stroke-width='2' stroke-dasharray='6 6'/>")
        if i == 0:
            body.append(f"<circle cx='{cx}' cy='{cy}' r='45' fill='{FORCE}' fill-opacity='0.08' "
                        f"stroke='{FORCE}' stroke-opacity='0.5' stroke-dasharray='3 4'/>")
            near = [(cx + 28, cy - 22), (cx + 30, cy + 20)]
            far = [(cx - 70, cy - 60), (cx - 80, cy + 55)]
            for nx, ny in near:
                body.append(f"<line x1='{cx}' y1='{cy}' x2='{nx}' y2='{ny}' stroke='{MUTED}' "
                            f"stroke-width='1.5' stroke-dasharray='3 3'/>")
                body.append(boid(nx, ny, -0.3, NEIGH))
            for nx, ny in far:
                body.append(boid(nx, ny, 0.2, NEIGH, opacity=0.45))
            body.append(arrow(cx, cy, cx - 70, cy, FORCE))
        elif i == 1:
            nbs = [(cx - 60, cy - 50, -0.5), (cx + 55, cy - 60, -0.7), (cx - 70, cy + 45, -0.4),
                   (cx + 60, cy + 50, -0.6), (cx + 15, cy - 80, -0.55)]
            for nx, ny, a in nbs:
                body.append(boid(nx, ny, a, NEIGH))
                body.append(arrow(nx, ny, nx + 26 * math.cos(a), ny + 26 * math.sin(a), MUTED, 1.5, "v"))
            body.append(arrow(cx, cy, cx + 70 * math.cos(-0.55), cy + 70 * math.sin(-0.55), FORCE))
        else:
            nbs = [(cx + 40, cy - 70), (cx + 75, cy - 25), (cx + 55, cy + 35), (cx + 85, cy + 10)]
            mx = sum(p[0] for p in nbs) / len(nbs)
            my = sum(p[1] for p in nbs) / len(nbs)
            for nx, ny in nbs:
                body.append(boid(nx, ny, 2.6, NEIGH))
            body.append(f"<circle cx='{mx:.1f}' cy='{my:.1f}' r='6' fill='none' stroke='{TEXT}' stroke-width='2'/>")
            body.append(f"<line x1='{mx - 9:.1f}' y1='{my:.1f}' x2='{mx + 9:.1f}' y2='{my:.1f}' stroke='{TEXT}' stroke-width='1.5'/>")
            body.append(f"<line x1='{mx:.1f}' y1='{my - 9:.1f}' x2='{mx:.1f}' y2='{my + 9:.1f}' stroke='{TEXT}' stroke-width='1.5'/>")
            body.append(arrow(cx, cy, cx + 0.8 * (mx - cx), cy + 0.8 * (my - cy), FORCE))
        body.append(boid(cx, cy, 0 if i != 1 else 0.6, FOCUS, 12))
    # legend
    lx, ly = 30, H - 28
    legend = [(FOCUS, "boid"), (NEIGH, "neighbour"), (FORCE, "steering force")]
    x = lx
    for c, label in legend:
        body.append(boid(x, ly - 5, 0, c, 8))
        body.append(text(x + 14, ly, label, 13, MUTED, anchor="start"))
        x += 30 + len(label) * 7.5
    body.append(text(W - 30, ly, "dashed circle = perception radius", 13, MUTED, anchor="end"))
    save("rules.svg", svg(W, H, "".join(body), defs))


# ---------------------------------------------------------------- 2. hash
def spatial_hash():
    W, H = 960, 420
    random.seed(7)
    cell, cols, rows, gx, gy = 50, 10, 7, 40, 40
    fx, fy = gx + 5 * cell + 22, gy + 3 * cell + 27  # focal boid in cell (5,3)
    body = [text(W - 250, 70, "Spatial hash grid", 24, TEXT, "bold")]
    for cxi in range(4, 7):
        for cyi in range(2, 5):
            body.append(f"<rect x='{gx + cxi * cell}' y='{gy + cyi * cell}' width='{cell}' "
                        f"height='{cell}' fill='{FOCUS}' fill-opacity='{0.28 if (cxi, cyi) == (5, 3) else 0.12}'/>")
    for c in range(cols + 1):
        body.append(f"<line x1='{gx + c * cell}' y1='{gy}' x2='{gx + c * cell}' y2='{gy + rows * cell}' stroke='{GRID}'/>")
    for r in range(rows + 1):
        body.append(f"<line x1='{gx}' y1='{gy + r * cell}' x2='{gx + cols * cell}' y2='{gy + r * cell}' stroke='{GRID}'/>")
    checked = skipped = 0
    for _ in range(80):
        x = random.uniform(gx + 5, gx + cols * cell - 5)
        y = random.uniform(gy + 5, gy + rows * cell - 5)
        if abs(x - fx) < 10 and abs(y - fy) < 10:
            continue
        inside = 4 <= int((x - gx) // cell) <= 6 and 2 <= int((y - gy) // cell) <= 4
        checked += inside
        skipped += not inside
        body.append(boid(x, y, random.uniform(-3.1, 3.1), NEIGH if inside else MUTED, 7, 1 if inside else 0.35))
    body.append(boid(fx, fy, 0.4, FOCUS, 11))
    tx = gx + cols * cell + 50
    lines = [
        ("The world is split into 100 px cells.", TEXT),
        ("Each frame every boid is filed into its cell.", TEXT),
        ("A boid only checks the 3 × 3 cells around it", TEXT),
        ("instead of every other boid.", TEXT),
        ("", TEXT),
        (f"<tspan fill='{NEIGH}'>●</tspan> checked here: {checked}", MUTED),
        (f"<tspan fill='{MUTED}'>●</tspan> skipped: {skipped}", MUTED),
        ("", TEXT),
        ("In the full app: 200 × 200 = 40 000 cells", MUTED),
        ("for up to 30 000 boids.", MUTED),
    ]
    for i, (s, c) in enumerate(lines):
        body.append(text(tx, 120 + i * 26, s, 15, c, anchor="start"))
    save("spatial-hash.svg", svg(W, H, "".join(body)))


# ---------------------------------------------------------------- 3. life
def lifecycle():
    W, H = 960, 340
    defs = marker("a", MUTED) + marker("d", FORCE) + marker("g", FOOD)
    body = [text(W / 2, 40, "Life cycle", 24, TEXT, "bold")]

    def node(x, y, w, title, sub, color):
        body.append(f"<rect x='{x - w / 2}' y='{y - 34}' width='{w}' height='68' rx='12' "
                    f"fill='{PANEL}' stroke='{color}' stroke-width='2'/>")
        body.append(text(x, y - 6, title, 17, color, "bold"))
        body.append(text(x, y + 18, sub, 13, MUTED))

    y1, y2 = 120, 250
    node(110, y1, 170, "Food", "eaten and stored", FOOD)
    node(345, y1, 190, "Energy", "food → energy over time", NEIGH)
    node(600, y1, 230, "Breed", "age > 20, energy > 1000, mate", FOCUS)
    node(845, y1, 190, "Child", "mutated genes, gen + 1", FOCUS)
    node(345, y2, 190, "Starve", "energy 0 → health drops", FORCE)
    node(600, y2, 230, "Die", "health 0 or age 90", FORCE)
    body.append(arrow(197, y1, 247, y1, MUTED, 2.5))
    body.append(arrow(442, y1, 482, y1, MUTED, 2.5))
    body.append(arrow(717, y1, 747, y1, MUTED, 2.5))
    body.append(arrow(345, y1 + 36, 345, y2 - 38, FORCE, 2.5, "d"))
    body.append(arrow(442, y2, 482, y2, FORCE, 2.5, "d"))
    body.append(f"<path d='M 600 {y2 + 36} L 600 {y2 + 62} L 110 {y2 + 62} L 110 {y1 + 38}' "
                f"fill='none' stroke='{FOOD}' stroke-width='2.5' stroke-dasharray='6 5' marker-end='url(#g)'/>")
    body.append(text(130, y2 + 54, "Food on Death: the body becomes food", 13, FOOD, anchor="start"))
    body.append(f"<path d='M 845 {y1 - 36} C 845 {y1 - 75}, 600 {y1 - 75}, 600 {y1 - 38}' fill='none' "
                f"stroke='{MUTED}' stroke-width='2' stroke-dasharray='4 4' marker-end='url(#a)'/>")
    body.append(text(722, y1 - 80, "grows up, eats, breeds", 12, MUTED))
    save("lifecycle.svg", svg(W, H, "".join(body), defs))


# ---------------------------------------------------------------- 4. hero
def hero():
    """Run a small boids simulation and draw the final frame."""
    W, H, N = 960, 360, 260
    random.seed(42)
    species = [(200, 1.0), (30, 1.0), (330, 1.0)]  # hue per gene group
    bs = []
    for i in range(N):
        hue = species[i % 3][0] + random.uniform(-18, 18)
        a = random.uniform(0, 2 * math.pi)
        bs.append([random.uniform(0, W), random.uniform(0, H), math.cos(a) * 2, math.sin(a) * 2, hue, i % 3])
    for _ in range(420):
        new = []
        for b in bs:
            ax = ay = 0.0
            cx = cy = vx = vy = sx = sy = 0.0
            n = 0
            for o in bs:
                if o is b:
                    continue
                dx, dy = o[0] - b[0], o[1] - b[1]
                d2 = dx * dx + dy * dy
                if d2 < 55 * 55 and (o[5] == b[5] or d2 < 14 * 14):
                    n += 1
                    cx += o[0]; cy += o[1]; vx += o[2]; vy += o[3]
                    if d2 < 14 * 14:
                        d = math.sqrt(d2) or 1
                        sx -= dx / d; sy -= dy / d
            if n:
                ax += (vx / n - b[2]) * 0.06 + (cx / n - b[0]) * 0.0025 + sx * 0.12
                ay += (vy / n - b[3]) * 0.06 + (cy / n - b[1]) * 0.0025 + sy * 0.12
            nvx, nvy = b[2] + ax, b[3] + ay
            sp = math.hypot(nvx, nvy) or 1
            sp2 = min(max(sp, 1.5), 3.0)
            nvx, nvy = nvx / sp * sp2, nvy / sp * sp2
            new.append([(b[0] + nvx) % W, (b[1] + nvy) % H, nvx, nvy, b[4], b[5]])
        bs = new
    body = []
    for _ in range(60):
        body.append(f"<circle cx='{random.uniform(10, W - 10):.1f}' cy='{random.uniform(10, H - 10):.1f}' "
                    f"r='2.2' fill='{FOOD}' fill-opacity='0.7'/>")
    for b in bs:
        ang = math.atan2(b[3], b[2])
        body.append(boid(b[0], b[1], ang, f"hsl({b[4]:.0f}, 85%, 62%)", 7))
    body.append(f"<rect x='16' y='{H - 44}' width='402' height='30' rx='8' fill='{BG}' fill-opacity='0.75'/>")
    body.append(text(28, H - 24, f"{N} boids · colour = genes · green dots = food", 14, MUTED, anchor="start"))
    save("flock.svg", svg(W, H, "".join(body)))


rules()
spatial_hash()
lifecycle()
hero()
print("ok")
