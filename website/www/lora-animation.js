import * as THREE from 'three';
import { EffectComposer } from 'three/addons/postprocessing/EffectComposer.js';
import { RenderPass } from 'three/addons/postprocessing/RenderPass.js';
import { UnrealBloomPass } from 'three/addons/postprocessing/UnrealBloomPass.js';

class LoRaAnimation {
    constructor(containerId) {
        this.container = document.getElementById(containerId);
        if (!this.container) return;

        // Scene Setup - Bright outdoor environment
        this.scene = new THREE.Scene();
        this.scene.background = new THREE.Color(0x87CEEB); // Sky blue
        this.scene.fog = new THREE.Fog(0x87CEEB, 30, 80);

        // Camera - Fixed position to capture everything
        this.camera = new THREE.PerspectiveCamera(
            50,
            this.container.clientWidth / this.container.clientHeight,
            0.1,
            200
        );
        this.camera.position.set(0, 12, 22);
        this.camera.lookAt(0, 2, 0);

        // Renderer with high quality settings
        this.renderer = new THREE.WebGLRenderer({
            antialias: true,
            alpha: false,
            powerPreference: "high-performance"
        });
        this.renderer.setSize(this.container.clientWidth, this.container.clientHeight);
        this.renderer.setPixelRatio(Math.min(window.devicePixelRatio, 2));
        this.renderer.shadowMap.enabled = true;
        this.renderer.shadowMap.type = THREE.PCFSoftShadowMap;
        this.renderer.toneMapping = THREE.ACESFilmicToneMapping;
        this.renderer.toneMappingExposure = 1.0;
        this.container.appendChild(this.renderer.domElement);

        // Subtle bloom for tech elements
        this.composer = new EffectComposer(this.renderer);
        this.composer.addPass(new RenderPass(this.scene, this.camera));

        const bloomPass = new UnrealBloomPass(
            new THREE.Vector2(this.container.clientWidth, this.container.clientHeight),
            0.6,  // Lower strength for realism
            0.4,
            0.9   // High threshold - only bright things glow
        );
        this.composer.addPass(bloomPass);

        // Animation state
        this.clock = new THREE.Clock();
        this.time = 0;
        this.phase = 0;
        this.phaseDuration = 5;

        this.people = [];
        this.satellites = [];
        this.effects = [];

        this.initLighting();
        this.initEnvironment();
        this.initPeople();
        this.initSatellites();
        this.initHUD();

        window.addEventListener('resize', () => this.onResize());
        this.animate();
    }

    initLighting() {
        // Bright ambient for visibility
        const ambient = new THREE.AmbientLight(0xffffff, 0.8);
        this.scene.add(ambient);

        // Warm sun light with shadows
        const sun = new THREE.DirectionalLight(0xfff5e6, 1.5);
        sun.position.set(10, 20, 10);
        sun.castShadow = true;
        sun.shadow.mapSize.width = 2048;
        sun.shadow.mapSize.height = 2048;
        sun.shadow.camera.near = 1;
        sun.shadow.camera.far = 50;
        sun.shadow.camera.left = -20;
        sun.shadow.camera.right = 20;
        sun.shadow.camera.top = 20;
        sun.shadow.camera.bottom = -20;
        this.scene.add(sun);

        // Fill light from opposite side
        const fill = new THREE.DirectionalLight(0x88ccff, 0.5);
        fill.position.set(-10, 10, -10);
        this.scene.add(fill);

        // Hemisphere light for realistic outdoor feel
        const hemi = new THREE.HemisphereLight(0x87CEEB, 0x3d5c3d, 0.4);
        this.scene.add(hemi);
    }

    initEnvironment() {
        // Ground - Grass texture simulation
        const groundGeo = new THREE.CircleGeometry(50, 64);
        const groundMat = new THREE.MeshStandardMaterial({
            color: 0x4a7c23,
            roughness: 0.9,
            metalness: 0
        });
        const ground = new THREE.Mesh(groundGeo, groundMat);
        ground.rotation.x = -Math.PI / 2;
        ground.receiveShadow = true;
        this.scene.add(ground);

        // Mountains in background
        this.createMountain(-25, 0, -30, 15, 0x5a6b4a);
        this.createMountain(25, 0, -35, 18, 0x4a5b3a);
        this.createMountain(0, 0, -40, 20, 0x6a7b5a);

        // Trees scattered around
        const treePositions = [
            [-8, 0, -5], [10, 0, -8], [-12, 0, 3], [15, 0, 5],
            [-6, 0, 8], [8, 0, -12], [-15, 0, -10], [12, 0, -3]
        ];
        treePositions.forEach(pos => this.createTree(pos[0], pos[1], pos[2]));
    }

    createMountain(x, y, z, height, color) {
        const geo = new THREE.ConeGeometry(height * 1.5, height, 8);
        const mat = new THREE.MeshStandardMaterial({
            color: color,
            roughness: 1,
            flatShading: true
        });
        const mountain = new THREE.Mesh(geo, mat);
        mountain.position.set(x, height / 2 - 2, z);
        this.scene.add(mountain);
    }

    createTree(x, y, z) {
        const trunk = new THREE.Mesh(
            new THREE.CylinderGeometry(0.15, 0.2, 1.5, 8),
            new THREE.MeshStandardMaterial({ color: 0x8B4513, roughness: 1 })
        );
        trunk.position.set(x, 0.75, z);
        trunk.castShadow = true;
        this.scene.add(trunk);

        const leaves = new THREE.Mesh(
            new THREE.ConeGeometry(1.2, 3, 8),
            new THREE.MeshStandardMaterial({ color: 0x228B22, roughness: 0.8, flatShading: true })
        );
        leaves.position.set(x, 3, z);
        leaves.castShadow = true;
        this.scene.add(leaves);
    }

    initPeople() {
        const positions = [
            { x: 0, z: 5, color: 0x3498db },   // Blue shirt
            { x: -4, z: -2, color: 0xe74c3c }, // Red shirt
            { x: 4, z: -2, color: 0x2ecc71 }   // Green shirt
        ];

        positions.forEach((p, idx) => {
            const person = this.createPerson(p.color);
            person.position.set(p.x, 0, p.z);
            person.userData.basePos = person.position.clone();
            person.userData.index = idx;
            this.scene.add(person);
            this.people.push(person);
        });
    }

    createPerson(shirtColor) {
        const group = new THREE.Group();

        // Body (torso)
        const torso = new THREE.Mesh(
            new THREE.CapsuleGeometry(0.35, 0.7, 4, 12),
            new THREE.MeshStandardMaterial({ color: shirtColor, roughness: 0.6 })
        );
        torso.position.y = 1.2;
        torso.castShadow = true;
        group.add(torso);

        // Head
        const head = new THREE.Mesh(
            new THREE.SphereGeometry(0.25, 16, 16),
            new THREE.MeshStandardMaterial({ color: 0xffdbac, roughness: 0.5 })
        );
        head.position.y = 1.85;
        head.castShadow = true;
        group.add(head);

        // Hair
        const hair = new THREE.Mesh(
            new THREE.SphereGeometry(0.27, 16, 8, 0, Math.PI * 2, 0, Math.PI / 2),
            new THREE.MeshStandardMaterial({ color: 0x3d2314, roughness: 0.8 })
        );
        hair.position.y = 1.9;
        group.add(hair);

        // Legs
        [-0.15, 0.15].forEach(xOff => {
            const leg = new THREE.Mesh(
                new THREE.CapsuleGeometry(0.12, 0.7, 4, 8),
                new THREE.MeshStandardMaterial({ color: 0x2c3e50, roughness: 0.7 })
            );
            leg.position.set(xOff, 0.45, 0);
            leg.castShadow = true;
            group.add(leg);
        });

        // Device in hand
        const device = new THREE.Mesh(
            new THREE.BoxGeometry(0.12, 0.2, 0.03),
            new THREE.MeshStandardMaterial({ color: 0x222222, roughness: 0.3, metalness: 0.8 })
        );
        device.position.set(0.4, 1.1, 0.2);
        device.rotation.z = -0.3;
        group.add(device);

        // Device screen (glows when active)
        const screen = new THREE.Mesh(
            new THREE.PlaneGeometry(0.08, 0.14),
            new THREE.MeshBasicMaterial({ color: 0x111111 })
        );
        screen.position.set(0.4, 1.1, 0.22);
        screen.rotation.z = -0.3;
        group.add(screen);

        // Ring indicator (for status visualization)
        const ring = new THREE.Mesh(
            new THREE.RingGeometry(0.6, 0.65, 32),
            new THREE.MeshBasicMaterial({
                color: 0x4A90E2,
                transparent: true,
                opacity: 0,
                side: THREE.DoubleSide
            })
        );
        ring.rotation.x = -Math.PI / 2;
        ring.position.y = 0.05;
        group.add(ring);

        group.userData.screen = screen;
        group.userData.ring = ring;

        return group;
    }

    initSatellites() {
        for (let i = 0; i < 3; i++) {
            const sat = new THREE.Group();

            // Body
            const body = new THREE.Mesh(
                new THREE.BoxGeometry(0.4, 0.4, 0.4),
                new THREE.MeshStandardMaterial({ color: 0xcccccc, metalness: 0.9, roughness: 0.2 })
            );
            sat.add(body);

            // Solar panels
            [-1, 1].forEach(side => {
                const panel = new THREE.Mesh(
                    new THREE.BoxGeometry(1.2, 0.05, 0.5),
                    new THREE.MeshStandardMaterial({
                        color: 0x1a237e,
                        metalness: 0.7,
                        roughness: 0.3,
                        emissive: 0x000033,
                        emissiveIntensity: 0.2
                    })
                );
                panel.position.x = side * 0.9;
                sat.add(panel);
            });

            // Status light
            const light = new THREE.Mesh(
                new THREE.SphereGeometry(0.05),
                new THREE.MeshBasicMaterial({ color: 0x00ff00 })
            );
            light.position.y = 0.25;
            sat.add(light);

            sat.position.set((i - 1) * 6, 15, -5);
            sat.userData.light = light;
            sat.userData.baseX = sat.position.x;

            this.scene.add(sat);
            this.satellites.push(sat);
        }
    }

    initHUD() {
        // Create canvas for HUD text
        this.hudCanvas = document.createElement('canvas');
        this.hudCanvas.width = 1024;
        this.hudCanvas.height = 256;
        this.hudCtx = this.hudCanvas.getContext('2d');
        this.hudTexture = new THREE.CanvasTexture(this.hudCanvas);

        const hudMat = new THREE.MeshBasicMaterial({
            map: this.hudTexture,
            transparent: true,
            depthTest: false
        });

        this.hud = new THREE.Mesh(new THREE.PlaneGeometry(14, 3.5), hudMat);
        this.hud.position.set(0, 9, 0);
        this.hud.renderOrder = 999;
        this.scene.add(this.hud);
    }

    updateHUD(title, subtitle = '') {
        const ctx = this.hudCtx;
        ctx.clearRect(0, 0, 1024, 256);

        if (!title) {
            this.hudTexture.needsUpdate = true;
            return;
        }

        // Semi-transparent background with rounded corners
        ctx.fillStyle = 'rgba(0, 30, 60, 0.85)';
        ctx.beginPath();
        ctx.roundRect(80, 30, 864, 196, 20);
        ctx.fill();

        // Border
        ctx.strokeStyle = '#4A90E2';
        ctx.lineWidth = 3;
        ctx.stroke();

        // Title
        ctx.font = 'bold 64px Arial, sans-serif';
        ctx.textAlign = 'center';
        ctx.fillStyle = '#ffffff';
        ctx.shadowColor = '#4A90E2';
        ctx.shadowBlur = 15;
        ctx.fillText(title, 512, 110);

        // Subtitle
        if (subtitle) {
            ctx.font = '36px Arial, sans-serif';
            ctx.fillStyle = '#50C878';
            ctx.shadowColor = '#50C878';
            ctx.shadowBlur = 10;
            ctx.fillText(subtitle, 512, 170);
        }

        this.hudTexture.needsUpdate = true;
    }

    spawnGPSBeam(satellite, person) {
        const start = satellite.position.clone();
        const end = person.position.clone();
        end.y = 2;

        // Beam line
        const points = [start, end];
        const lineGeo = new THREE.BufferGeometry().setFromPoints(points);
        const lineMat = new THREE.LineBasicMaterial({
            color: 0xffcc00,
            transparent: true,
            opacity: 0.6
        });
        const line = new THREE.Line(lineGeo, lineMat);
        this.scene.add(line);

        // Data packet
        const packet = new THREE.Mesh(
            new THREE.SphereGeometry(0.15),
            new THREE.MeshBasicMaterial({ color: 0xffcc00 })
        );
        packet.position.copy(start);
        this.scene.add(packet);

        this.effects.push({
            type: 'beam',
            line: line,
            packet: packet,
            start: start,
            end: end,
            progress: 0
        });
    }

    spawnLoRaWave(person) {
        const wave = new THREE.Mesh(
            new THREE.RingGeometry(0.3, 0.4, 32),
            new THREE.MeshBasicMaterial({
                color: 0x50C878,
                transparent: true,
                opacity: 1,
                side: THREE.DoubleSide
            })
        );
        wave.rotation.x = -Math.PI / 2;
        wave.position.copy(person.position);
        wave.position.y = 1;
        this.scene.add(wave);

        this.effects.push({
            type: 'wave',
            mesh: wave,
            scale: 1,
            maxScale: 12
        });
    }

    spawnP2PPacket(from, to) {
        const start = from.position.clone();
        start.y = 1.5;
        const end = to.position.clone();
        end.y = 1.5;

        // Connection line
        const points = [start, end];
        const lineGeo = new THREE.BufferGeometry().setFromPoints(points);
        const lineMat = new THREE.LineBasicMaterial({
            color: 0x50C878,
            transparent: true,
            opacity: 0.4
        });
        const line = new THREE.Line(lineGeo, lineMat);
        this.scene.add(line);

        // Packet
        const packet = new THREE.Mesh(
            new THREE.SphereGeometry(0.12),
            new THREE.MeshBasicMaterial({ color: 0x50C878 })
        );
        packet.position.copy(start);
        this.scene.add(packet);

        this.effects.push({
            type: 'p2p',
            line: line,
            packet: packet,
            start: start,
            end: end,
            progress: 0
        });
    }

    animate() {
        requestAnimationFrame(() => this.animate());

        const delta = this.clock.getDelta();
        this.time += delta;

        // Calculate current phase (0-4)
        this.phase = Math.floor(this.time / this.phaseDuration) % 5;
        const phaseTime = this.time % this.phaseDuration;

        // Gentle camera movement
        const cameraAngle = Math.sin(this.time * 0.15) * 0.3;
        this.camera.position.x = Math.sin(cameraAngle) * 22;
        this.camera.position.z = Math.cos(cameraAngle) * 22;
        this.camera.lookAt(0, 2, 0);
        this.hud.lookAt(this.camera.position);

        // Phase logic
        this.handlePhase(phaseTime);

        // Animate satellites
        this.satellites.forEach((sat, i) => {
            sat.position.x = sat.userData.baseX + Math.sin(this.time * 0.3 + i) * 2;
            sat.rotation.y = this.time * 0.2;

            // Blink light
            sat.userData.light.material.color.setHex(
                Math.sin(this.time * 5 + i) > 0 ? 0x00ff00 : 0x003300
            );
        });

        // Idle animation for people
        this.people.forEach((p, i) => {
            p.position.y = Math.sin(this.time * 2 + i) * 0.02;
        });

        // Update effects
        this.updateEffects(delta);

        this.composer.render();
    }

    handlePhase(phaseTime) {
        switch (this.phase) {
            case 0: // Intro
                if (phaseTime < 0.1) {
                    this.updateHUD('');
                    this.resetVisuals();
                }
                break;

            case 1: // GPS Acquisition
                if (phaseTime < 0.1) {
                    this.updateHUD('GPS LOCK', 'Acquiring satellite position...');
                    this.people.forEach(p => {
                        p.userData.screen.material.color.setHex(0xffcc00);
                    });
                }
                if (Math.random() < 0.08) {
                    const sat = this.satellites[Math.floor(Math.random() * 3)];
                    const person = this.people[Math.floor(Math.random() * 3)];
                    this.spawnGPSBeam(sat, person);
                }
                break;

            case 2: // LoRa Connection
                if (phaseTime < 0.1) {
                    this.updateHUD('LORA ACTIVE', 'Long-range radio connected');
                    this.people.forEach(p => {
                        p.userData.screen.material.color.setHex(0x50C878);
                        p.userData.ring.material.opacity = 0.3;
                    });
                }
                if (Math.random() < 0.06) {
                    const person = this.people[Math.floor(Math.random() * 3)];
                    this.spawnLoRaWave(person);
                }
                break;

            case 3: // Mesh Network
                if (phaseTime < 0.1) {
                    this.updateHUD('MESH NETWORK', 'Real-time location sharing');
                }
                if (Math.random() < 0.08) {
                    const from = this.people[Math.floor(Math.random() * 3)];
                    let to = this.people[Math.floor(Math.random() * 3)];
                    while (to === from) to = this.people[Math.floor(Math.random() * 3)];
                    this.spawnP2PPacket(from, to);
                }
                break;

            case 4: // Finale
                if (phaseTime < 0.1) {
                    this.updateHUD('OFF-GRID FREEDOM', 'No cellular network required');
                }
                break;
        }
    }

    updateEffects(delta) {
        for (let i = this.effects.length - 1; i >= 0; i--) {
            const e = this.effects[i];

            if (e.type === 'beam' || e.type === 'p2p') {
                e.progress += delta * 1.2;
                e.packet.position.lerpVectors(e.start, e.end, Math.min(e.progress, 1));

                if (e.progress >= 1.2) {
                    this.scene.remove(e.line);
                    this.scene.remove(e.packet);
                    this.effects.splice(i, 1);
                }
            } else if (e.type === 'wave') {
                e.scale += delta * 4;
                e.mesh.scale.set(e.scale, e.scale, 1);
                e.mesh.material.opacity = 1 - (e.scale / e.maxScale);

                if (e.scale >= e.maxScale) {
                    this.scene.remove(e.mesh);
                    this.effects.splice(i, 1);
                }
            }
        }
    }

    resetVisuals() {
        this.people.forEach(p => {
            p.userData.screen.material.color.setHex(0x111111);
            p.userData.ring.material.opacity = 0;
        });
    }

    onResize() {
        if (!this.container) return;
        const w = this.container.clientWidth;
        const h = this.container.clientHeight;
        this.camera.aspect = w / h;
        this.camera.updateProjectionMatrix();
        this.renderer.setSize(w, h);
        this.composer.setSize(w, h);
    }
}

export function initLoRaAnimation(containerId) {
    return new LoRaAnimation(containerId);
}
