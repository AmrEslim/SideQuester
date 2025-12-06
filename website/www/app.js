// Import the WASM module
import init, { 
    CompassCalculator, 
    calculate_distance, 
    calculate_bearing, 
    get_direction,
    format_distance,
    calculate_relative_bearing
} from './pkg/sidequester_wasm.js';

// Global state
let compass = null;
let currentHeading = 0;
let targetBearing = 0;
let distance = 0;
let canvas = null;
let ctx = null;

// Initialize the demo
async function initDemo() {
    try {
        // Load WASM module
        await init();
        console.log('✓ WASM module loaded successfully');
        
        // Create compass calculator
        compass = new CompassCalculator();
        console.log('✓ Compass calculator initialized');
        
        // Hide loading overlay
        document.getElementById('loadingOverlay').style.display = 'none';
        
        // Setup canvas
        canvas = document.getElementById('compassCanvas');
        ctx = canvas.getContext('2d');
        
        // Setup event listeners
        setupEventListeners();
        
        // Initial calculation
        calculatePositions();
        
        // Start animation loop
        animate();
        
        console.log('✓ Demo ready!');
    } catch (error) {
        console.error('Failed to initialize demo:', error);
        alert('Failed to load WebAssembly module. Please refresh the page.');
    }
}

// Setup all event listeners
function setupEventListeners() {
    // Heading slider
    const headingSlider = document.getElementById('currentHeading');
    const headingValue = document.getElementById('headingValue');
    
    headingSlider.addEventListener('input', (e) => {
        currentHeading = parseFloat(e.target.value);
        headingValue.textContent = `${currentHeading}°`;
        updateDisplay();
    });
    
    // Smoothing slider
    const smoothingSlider = document.getElementById('smoothing');
    const smoothingValue = document.getElementById('smoothingValue');
    
    smoothingSlider.addEventListener('input', (e) => {
        const factor = parseFloat(e.target.value) / 100;
        smoothingValue.textContent = factor.toFixed(2);
        if (compass) {
            compass.set_smoothing(factor);
        }
    });
    
    // Reset button
    document.getElementById('resetBtn').addEventListener('click', () => {
        if (compass) {
            compass.reset();
            currentHeading = 0;
            headingSlider.value = 0;
            headingValue.textContent = '0°';
            updateDisplay();
        }
    });
    
    // Calculate button
    document.getElementById('calculateBtn').addEventListener('click', () => {
        calculatePositions();
    });
    
    // Preset buttons
    document.querySelectorAll('.btn-preset').forEach(btn => {
        btn.addEventListener('click', (e) => {
            const preset = e.target.dataset.preset;
            applyPreset(preset);
        });
    });
}

// Calculate distance and bearing from GPS coordinates
function calculatePositions() {
    const yourLat = parseFloat(document.getElementById('yourLat').value) || 0;
    const yourLon = parseFloat(document.getElementById('yourLon').value) || 0;
    const friendLat = parseFloat(document.getElementById('friendLat').value) || 0;
    const friendLon = parseFloat(document.getElementById('friendLon').value) || 0;
    
    // Calculate distance and bearing using WASM functions
    distance = calculate_distance(yourLat, yourLon, friendLat, friendLon);
    targetBearing = calculate_bearing(yourLat, yourLon, friendLat, friendLon);
    
    updateDisplay();
}

// Apply preset locations
function applyPreset(preset) {
    const presets = {
        nearby: {
            yourLat: 30.0444,
            yourLon: 31.2357,
            friendLat: 30.0489,
            friendLon: 31.2390
        },
        festival: {
            yourLat: 30.0444,
            yourLon: 31.2357,
            friendLat: 30.0600,
            friendLon: 31.2500
        },
        far: {
            yourLat: 30.0444,
            yourLon: 31.2357,
            friendLat: 30.1444,
            friendLon: 31.3357
        }
    };
    
    const coords = presets[preset];
    if (coords) {
        document.getElementById('yourLat').value = coords.yourLat;
        document.getElementById('yourLon').value = coords.yourLon;
        document.getElementById('friendLat').value = coords.friendLat;
        document.getElementById('friendLon').value = coords.friendLon;
        calculatePositions();
    }
}

// Update all display values
function updateDisplay() {
    // Update text displays
    document.getElementById('heading').textContent = `${currentHeading.toFixed(1)}°`;
    document.getElementById('direction').textContent = get_direction(currentHeading);
    document.getElementById('distance').textContent = format_distance(distance);
    document.getElementById('bearing').textContent = `${targetBearing.toFixed(1)}°`;
    
    // Redraw compass
    drawCompass();
}

// Draw the compass on canvas
function drawCompass() {
    if (!ctx || !canvas) return;
    
    const centerX = canvas.width / 2;
    const centerY = canvas.height / 2;
    const radius = 150;
    
    // Clear canvas
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    
    // Draw outer circle
    ctx.strokeStyle = '#4A90E2';
    ctx.lineWidth = 3;
    ctx.beginPath();
    ctx.arc(centerX, centerY, radius, 0, Math.PI * 2);
    ctx.stroke();
    
    // Draw background circle
    ctx.fillStyle = 'rgba(74, 144, 226, 0.05)';
    ctx.beginPath();
    ctx.arc(centerX, centerY, radius, 0, Math.PI * 2);
    ctx.fill();
    
    // Save context for rotation
    ctx.save();
    ctx.translate(centerX, centerY);
    ctx.rotate(-currentHeading * Math.PI / 180);
    
    // Draw cardinal directions
    drawCardinalDirections(ctx, radius);
    
    // Draw degree markers
    drawDegreeMarkers(ctx, radius);
    
    ctx.restore();
    
    // Draw center dot
    ctx.fillStyle = '#4A90E2';
    ctx.beginPath();
    ctx.arc(centerX, centerY, 8, 0, Math.PI * 2);
    ctx.fill();
    
    // Draw target bearing arrow
    if (distance > 0) {
        drawTargetArrow(ctx, centerX, centerY, radius, targetBearing);
    }
    
    // Draw heading indicator
    drawHeadingIndicator(ctx, centerX, centerY, radius);
}

// Draw cardinal directions (N, E, S, W)
function drawCardinalDirections(ctx, radius) {
    const directions = [
        { text: 'N', angle: 0, color: '#FF6B6B' },
        { text: 'E', angle: 90, color: '#4A90E2' },
        { text: 'S', angle: 180, color: '#4A90E2' },
        { text: 'W', angle: 270, color: '#4A90E2' }
    ];
    
    ctx.font = 'bold 24px Arial';
    ctx.textAlign = 'center';
    ctx.textBaseline = 'middle';
    
    directions.forEach(dir => {
        ctx.save();
        ctx.rotate(dir.angle * Math.PI / 180);
        ctx.fillStyle = dir.color;
        ctx.fillText(dir.text, 0, -radius + 30);
        ctx.restore();
    });
}

// Draw degree markers
function drawDegreeMarkers(ctx, radius) {
    ctx.strokeStyle = '#ccc';
    ctx.lineWidth = 1;
    
    for (let i = 0; i < 360; i += 10) {
        ctx.save();
        ctx.rotate(i * Math.PI / 180);
        
        const isMajor = i % 30 === 0;
        const markerLength = isMajor ? 15 : 8;
        
        ctx.beginPath();
        ctx.moveTo(0, -radius);
        ctx.lineTo(0, -radius + markerLength);
        ctx.stroke();
        
        ctx.restore();
    }
}

// Draw target bearing arrow (where your friend is)
function drawTargetArrow(ctx, centerX, centerY, radius, bearing) {
    const relativeBearing = calculate_relative_bearing(currentHeading, bearing);
    const angle = relativeBearing * Math.PI / 180;
    
    ctx.save();
    ctx.translate(centerX, centerY);
    ctx.rotate(angle);
    
    // Draw arrow
    ctx.fillStyle = '#FF6B6B';
    ctx.beginPath();
    ctx.moveTo(0, -radius + 40);
    ctx.lineTo(-12, -radius + 70);
    ctx.lineTo(12, -radius + 70);
    ctx.closePath();
    ctx.fill();
    
    // Draw arrow shaft
    ctx.fillRect(-3, -radius + 70, 6, 40);
    
    ctx.restore();
}

// Draw heading indicator at top
function drawHeadingIndicator(ctx, centerX, centerY, radius) {
    ctx.save();
    ctx.translate(centerX, centerY);
    
    // Draw small triangle at top
    ctx.fillStyle = '#4A90E2';
    ctx.beginPath();
    ctx.moveTo(0, -radius - 15);
    ctx.lineTo(-8, -radius - 5);
    ctx.lineTo(8, -radius - 5);
    ctx.closePath();
    ctx.fill();
    
    ctx.restore();
}

// Animation loop
function animate() {
    // No continuous animation needed, only update on user input
    requestAnimationFrame(animate);
}

// Initialize when DOM is loaded
if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', initDemo);
} else {
    initDemo();
}
