class Particle {
    constructor(x, y, targetX, targetY, color) {
        this.x = x;
        this.y = y;
        this.targetX = targetX;
        this.targetY = targetY;
        this.progress = 0;
        this.color = color;
        this.size = Math.random() * 3 + 2;
        this.speed = 0.02 + Math.random() * 0.03;
        this.alpha = 1;
    }

    update() {
        this.progress += this.speed;
        if (this.progress >= 1) {
            this.progress = 1;
            this.alpha -= 0.05;
        }

        // Smooth easing
        const eased = this.easeInOutCubic(this.progress);
        this.x = this.x + (this.targetX - this.x) * eased;
        this.y = this.y + (this.targetY - this.y) * eased;
    }

    easeInOutCubic(t) {
        return t < 0.5 ? 4 * t * t * t : 1 - Math.pow(-2 * t + 2, 3) / 2;
    }

    draw(ctx) {
        if (this.alpha <= 0) return;

        ctx.save();
        ctx.globalAlpha = this.alpha;

        // Outer glow
        const gradient = ctx.createRadialGradient(this.x, this.y, 0, this.x, this.y, this.size * 3);
        gradient.addColorStop(0, this.color);
        gradient.addColorStop(0.5, this.color.replace('1)', '0.3)'));
        gradient.addColorStop(1, 'transparent');

        ctx.fillStyle = gradient;
        ctx.beginPath();
        ctx.arc(this.x, this.y, this.size * 3, 0, Math.PI * 2);
        ctx.fill();

        // Core
        ctx.fillStyle = this.color;
        ctx.beginPath();
        ctx.arc(this.x, this.y, this.size, 0, Math.PI * 2);
        ctx.fill();

        ctx.restore();
    }

    isDead() {
        return this.alpha <= 0;
    }
}

class SecurityLaserVisualizer {
    constructor() {
        this.canvas = document.getElementById('gridCanvas');
        this.ctx = this.canvas.getContext('2d');
        this.rows = 6;
        this.cols = 6;
        this.cellSize = 60;
        this.grid = [];
        this.beams = [];
        this.particles = [];
        this.isAnimating = false;
        this.animationProgress = 0;
        this.currentBeamProgress = 0;
        this.pulsePhase = 0;
        this.scanningRow = -1;

        this.colors = {
            device: 'rgba(255, 0, 85, 1)',
            deviceGlow: 'rgba(255, 0, 85, 0.8)',
            beam: 'rgba(0, 217, 255, 1)',
            beamGlow: 'rgba(0, 217, 255, 0.6)',
            particle: 'rgba(0, 217, 255, 1)',
            empty: 'rgba(20, 30, 50, 0.3)',
            grid: 'rgba(0, 217, 255, 0.2)',
            highlight: 'rgba(255, 215, 0, 0.5)',
            scanning: 'rgba(0, 255, 136, 0.3)'
        };

        this.initGrid(true);
        this.setupEventListeners();
        this.startAnimationLoop();
    }

    initGrid(loadExample = false) {
        this.grid = [];
        for (let i = 0; i < this.rows; i++) {
            this.grid[i] = [];
            for (let j = 0; j < this.cols; j++) {
                this.grid[i][j] = 0;
            }
        }
        this.updateCanvasSize();

        if (loadExample) {
            this.loadPreset('example1');
        }
    }

    updateCanvasSize() {
        this.canvas.width = this.cols * this.cellSize + 1;
        this.canvas.height = this.rows * this.cellSize + 1;
    }

    setupEventListeners() {
        this.canvas.addEventListener('click', (e) => this.handleCanvasClick(e));

        document.getElementById('createGrid').addEventListener('click', () => {
            this.rows = parseInt(document.getElementById('rows').value);
            this.cols = parseInt(document.getElementById('cols').value);
            this.initGrid();
            this.updateStats();
        });

        document.getElementById('calculate').addEventListener('click', () => {
            this.calculateBeams();
            this.updateStats();
        });

        document.getElementById('animate').addEventListener('click', () => {
            this.animateCalculation();
        });

        document.getElementById('random').addEventListener('click', () => {
            this.randomizeGrid();
            this.updateStats();
        });

        document.getElementById('clear').addEventListener('click', () => {
            this.initGrid();
            this.beams = [];
            this.particles = [];
            this.updateStats();
        });

        document.getElementById('presets').addEventListener('change', (e) => {
            this.loadPreset(e.target.value);
        });
    }

    handleCanvasClick(e) {
        if (this.isAnimating) return;

        const rect = this.canvas.getBoundingClientRect();
        const x = e.clientX - rect.left;
        const y = e.clientY - rect.top;

        const col = Math.floor(x / this.cellSize);
        const row = Math.floor(y / this.cellSize);

        if (row >= 0 && row < this.rows && col >= 0 && col < this.cols) {
            this.grid[row][col] = this.grid[row][col] === 0 ? 1 : 0;
            this.beams = [];
            this.particles = [];
            this.updateStats();
        }
    }

    startAnimationLoop() {
        const animate = () => {
            this.pulsePhase += 0.03;
            this.updateParticles();
            this.draw();
            requestAnimationFrame(animate);
        };
        animate();
    }

    updateParticles() {
        for (let i = this.particles.length - 1; i >= 0; i--) {
            this.particles[i].update();
            if (this.particles[i].isDead()) {
                this.particles.splice(i, 1);
            }
        }
    }

    draw() {
        this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);

        // Draw grid background
        for (let i = 0; i < this.rows; i++) {
            for (let j = 0; j < this.cols; j++) {
                const x = j * this.cellSize;
                const y = i * this.cellSize;

                // Scanning row highlight
                if (i === this.scanningRow && this.isAnimating) {
                    this.ctx.fillStyle = this.colors.scanning;
                    this.ctx.fillRect(x + 1, y + 1, this.cellSize - 1, this.cellSize - 1);
                } else if (this.grid[i][j] === 1) {
                    this.drawDevice(x, y);
                } else {
                    this.ctx.fillStyle = this.colors.empty;
                    this.ctx.fillRect(x + 1, y + 1, this.cellSize - 1, this.cellSize - 1);
                }

                // Grid lines
                this.ctx.strokeStyle = this.colors.grid;
                this.ctx.lineWidth = 1;
                this.ctx.strokeRect(x, y, this.cellSize, this.cellSize);
            }
        }

        // Draw beams with glow
        this.drawBeams();

        // Draw particles
        this.particles.forEach(particle => particle.draw(this.ctx));
    }

    drawDevice(x, y) {
        const centerX = x + this.cellSize / 2;
        const centerY = y + this.cellSize / 2;
        const size = this.cellSize * 0.4;

        // Pulsing glow effect
        const pulseIntensity = Math.sin(this.pulsePhase) * 0.3 + 0.7;
        const glowSize = size * (1 + pulseIntensity * 0.5);

        // Outer glow layers
        for (let i = 3; i > 0; i--) {
            const gradient = this.ctx.createRadialGradient(
                centerX, centerY, size,
                centerX, centerY, glowSize * (1 + i * 0.3)
            );
            gradient.addColorStop(0, this.colors.deviceGlow);
            gradient.addColorStop(1, 'transparent');

            this.ctx.fillStyle = gradient;
            this.ctx.beginPath();
            this.ctx.arc(centerX, centerY, glowSize * (1 + i * 0.3), 0, Math.PI * 2);
            this.ctx.fill();
        }

        // Main device body (3D effect)
        const gradient = this.ctx.createRadialGradient(
            centerX - size * 0.3, centerY - size * 0.3, size * 0.2,
            centerX, centerY, size
        );
        gradient.addColorStop(0, '#ff3377');
        gradient.addColorStop(0.5, this.colors.device);
        gradient.addColorStop(1, '#880033');

        this.ctx.fillStyle = gradient;
        this.ctx.beginPath();
        this.ctx.arc(centerX, centerY, size, 0, Math.PI * 2);
        this.ctx.fill();

        // Highlight
        const highlightGradient = this.ctx.createRadialGradient(
            centerX - size * 0.3, centerY - size * 0.3, 0,
            centerX - size * 0.3, centerY - size * 0.3, size * 0.6
        );
        highlightGradient.addColorStop(0, 'rgba(255, 255, 255, 0.6)');
        highlightGradient.addColorStop(1, 'transparent');

        this.ctx.fillStyle = highlightGradient;
        this.ctx.beginPath();
        this.ctx.arc(centerX - size * 0.2, centerY - size * 0.2, size * 0.4, 0, Math.PI * 2);
        this.ctx.fill();

        // Scanning circle animation
        this.ctx.strokeStyle = this.colors.beamGlow;
        this.ctx.lineWidth = 2;
        const scanRadius = size * (0.5 + Math.sin(this.pulsePhase * 2) * 0.3);
        this.ctx.beginPath();
        this.ctx.arc(centerX, centerY, scanRadius, 0, Math.PI * 2);
        this.ctx.stroke();
    }

    drawBeams() {
        if (this.beams.length === 0) return;

        this.beams.forEach((beam, index) => {
            const x1 = beam.col1 * this.cellSize + this.cellSize / 2;
            const y1 = beam.row1 * this.cellSize + this.cellSize / 2;
            const x2 = beam.col2 * this.cellSize + this.cellSize / 2;
            const y2 = beam.row2 * this.cellSize + this.cellSize / 2;

            // Beam progress for animation
            const progress = beam.progress || 1;
            const currentX2 = x1 + (x2 - x1) * progress;
            const currentY2 = y1 + (y2 - y1) * progress;

            // Pulsing intensity
            const pulseIntensity = Math.sin(this.pulsePhase + index * 0.5) * 0.3 + 0.7;

            // Multiple glow layers for depth
            for (let i = 3; i > 0; i--) {
                this.ctx.strokeStyle = this.colors.beamGlow.replace('0.6', (0.15 * i * pulseIntensity).toString());
                this.ctx.lineWidth = 15 * i;
                this.ctx.lineCap = 'round';

                this.ctx.beginPath();
                this.ctx.moveTo(x1, y1);
                this.ctx.lineTo(currentX2, currentY2);
                this.ctx.stroke();
            }

            // Core beam (bright)
            this.ctx.strokeStyle = this.colors.beam;
            this.ctx.lineWidth = 4;
            this.ctx.shadowBlur = 20;
            this.ctx.shadowColor = this.colors.beam;

            this.ctx.beginPath();
            this.ctx.moveTo(x1, y1);
            this.ctx.lineTo(currentX2, currentY2);
            this.ctx.stroke();

            this.ctx.shadowBlur = 0;

            // Inner bright line
            this.ctx.strokeStyle = 'rgba(255, 255, 255, 0.8)';
            this.ctx.lineWidth = 1;

            this.ctx.beginPath();
            this.ctx.moveTo(x1, y1);
            this.ctx.lineTo(currentX2, currentY2);
            this.ctx.stroke();
        });
    }

    calculateBeams() {
        this.beams = [];
        let prevRow = -1;
        let prevDevices = [];

        for (let i = 0; i < this.rows; i++) {
            const currentDevices = [];

            for (let j = 0; j < this.cols; j++) {
                if (this.grid[i][j] === 1) {
                    currentDevices.push(j);
                }
            }

            if (currentDevices.length > 0 && prevRow !== -1) {
                for (const prevCol of prevDevices) {
                    for (const currCol of currentDevices) {
                        this.beams.push({
                            row1: prevRow,
                            col1: prevCol,
                            row2: i,
                            col2: currCol,
                            progress: 1
                        });
                    }
                }
            }

            if (currentDevices.length > 0) {
                prevRow = i;
                prevDevices = currentDevices;
            }
        }

        this.updateStats();
        return this.beams.length;
    }

    async animateCalculation() {
        if (this.isAnimating) return;

        // Check for devices
        let hasDevices = false;
        for (let i = 0; i < this.rows; i++) {
            for (let j = 0; j < this.cols; j++) {
                if (this.grid[i][j] === 1) {
                    hasDevices = true;
                    break;
                }
            }
            if (hasDevices) break;
        }

        if (!hasDevices) {
            alert('No devices on the grid! Loading Example 1 for you...');
            this.loadPreset('example1');
            await this.sleep(500);
        }

        this.isAnimating = true;
        this.beams = [];
        this.particles = [];

        // Show animation info panel
        const infoPanel = document.getElementById('animationInfo');
        const stepDiv = document.getElementById('animationStep');
        const progressFill = document.getElementById('progressFill');
        infoPanel.style.display = 'block';

        let prevRow = -1;
        let prevDevices = [];
        let totalBeams = 0;
        let rowsProcessed = 0;

        for (let i = 0; i < this.rows; i++) {
            this.scanningRow = i;
            const currentDevices = [];

            // Find devices in current row
            for (let j = 0; j < this.cols; j++) {
                if (this.grid[i][j] === 1) {
                    currentDevices.push(j);
                }
            }

            // Update info panel
            const progress = ((i + 1) / this.rows * 100).toFixed(0);
            progressFill.style.width = progress + '%';

            if (currentDevices.length > 0) {
                stepDiv.innerHTML = `
                    <strong>📡 Scanning Row ${i}</strong><br>
                    Found ${currentDevices.length} device(s) at positions: [${currentDevices.join(', ')}]<br>
                    ${prevRow !== -1 ? `Connecting to Row ${prevRow} (${prevDevices.length} devices)...` : 'Waiting for next row with devices...'}
                `;
            } else {
                stepDiv.innerHTML = `
                    <strong>📡 Scanning Row ${i}</strong><br>
                    No devices found - skipping...<br>
                    <span style="opacity: 0.6;">Empty rows don't break existing beam paths</span>
                `;
            }

            await this.sleep(600);

            // Create beams with animation
            if (currentDevices.length > 0 && prevRow !== -1) {
                const beamsThisStep = prevDevices.length * currentDevices.length;
                stepDiv.innerHTML += `<br><strong style="color: #ffd700;">⚡ Creating ${beamsThisStep} beam(s): ${prevDevices.length} × ${currentDevices.length}</strong>`;

                for (const prevCol of prevDevices) {
                    for (const currCol of currentDevices) {
                        const beam = {
                            row1: prevRow,
                            col1: prevCol,
                            row2: i,
                            col2: currCol,
                            progress: 0
                        };
                        this.beams.push(beam);

                        // Animate beam construction
                        await this.animateBeam(beam);

                        // Create particle effects
                        this.createParticlesBurst(beam);

                        totalBeams++;
                        this.updateStats();
                        await this.sleep(400);
                    }
                }
            }

            if (currentDevices.length > 0) {
                prevRow = i;
                prevDevices = currentDevices;
            }

            rowsProcessed++;
        }

        this.scanningRow = -1;
        stepDiv.innerHTML = `
            <strong style="color: #00ff88;">✅ Analysis Complete!</strong><br>
            Processed ${rowsProcessed} rows<br>
            Created <strong style="color: #ffd700;">${totalBeams} laser beams</strong><br>
            <span style="opacity: 0.8;">Total connections established successfully</span>
        `;

        await this.sleep(2000);
        infoPanel.style.display = 'none';
        this.isAnimating = false;
    }

    async animateBeam(beam) {
        const steps = 20;
        for (let step = 0; step <= steps; step++) {
            beam.progress = step / steps;
            await this.sleep(20);
        }
    }

    createParticlesBurst(beam) {
        const x1 = beam.col1 * this.cellSize + this.cellSize / 2;
        const y1 = beam.row1 * this.cellSize + this.cellSize / 2;
        const x2 = beam.col2 * this.cellSize + this.cellSize / 2;
        const y2 = beam.row2 * this.cellSize + this.cellSize / 2;

        // Create particles along the beam
        for (let i = 0; i < 10; i++) {
            const t = Math.random();
            const particleX = x1 + (x2 - x1) * t;
            const particleY = y1 + (y2 - y1) * t;

            this.particles.push(new Particle(
                particleX,
                particleY,
                x2,
                y2,
                this.colors.particle
            ));
        }
    }

    updateStats() {
        let deviceCount = 0;
        for (let i = 0; i < this.rows; i++) {
            for (let j = 0; j < this.cols; j++) {
                if (this.grid[i][j] === 1) deviceCount++;
            }
        }

        const beamCount = this.beams.length;
        const avgBeams = deviceCount > 0 ? (beamCount / deviceCount).toFixed(2) : '0.00';

        // Animated counter effect
        this.animateNumber('totalDevices', deviceCount);
        this.animateNumber('totalBeams', beamCount);
        document.getElementById('avgBeams').textContent = avgBeams;
    }

    animateNumber(elementId, target) {
        const element = document.getElementById(elementId);
        const current = parseInt(element.textContent) || 0;
        const increment = (target - current) / 10;

        let count = 0;
        const timer = setInterval(() => {
            count++;
            const value = Math.round(current + increment * count);
            element.textContent = value;

            if (count >= 10) {
                element.textContent = target;
                clearInterval(timer);
            }
        }, 30);
    }

    randomizeGrid() {
        const density = 0.25;
        for (let i = 0; i < this.rows; i++) {
            for (let j = 0; j < this.cols; j++) {
                this.grid[i][j] = Math.random() < density ? 1 : 0;
            }
        }
        this.beams = [];
        this.particles = [];
    }

    loadPreset(preset) {
        if (!preset) return;

        const presets = {
            example1: ["011001", "000000", "010100", "001000"],
            example2: ["000", "111", "000"],
            maxBeams: ["111", "000", "111"],
            sparse: ["10000", "00000", "00001", "00000", "01000"]
        };

        const data = presets[preset];
        if (data) {
            this.rows = data.length;
            this.cols = data[0].length;
            this.grid = [];

            for (let i = 0; i < this.rows; i++) {
                this.grid[i] = [];
                for (let j = 0; j < this.cols; j++) {
                    this.grid[i][j] = parseInt(data[i][j]);
                }
            }

            document.getElementById('rows').value = this.rows;
            document.getElementById('cols').value = this.cols;

            this.updateCanvasSize();
            this.beams = [];
            this.particles = [];
            this.updateStats();
        }
    }

    sleep(ms) {
        return new Promise(resolve => setTimeout(resolve, ms));
    }
}

// Initialize when page loads
document.addEventListener('DOMContentLoaded', () => {
    new SecurityLaserVisualizer();
});
