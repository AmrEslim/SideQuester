use wasm_bindgen::prelude::*;
use std::f64::consts::PI;

/// Main compass calculator for SideQuester
/// Implements the same algorithms used in the Arduino firmware
#[wasm_bindgen]
pub struct CompassCalculator {
    smoothing_factor: f64,
    last_heading: f64,
}

#[wasm_bindgen]
impl CompassCalculator {
    /// Create a new compass calculator
    #[wasm_bindgen(constructor)]
    pub fn new() -> CompassCalculator {
        CompassCalculator {
            smoothing_factor: 0.08,
            last_heading: 0.0,
        }
    }

    /// Calculate heading from magnetometer X and Y values
    /// Returns heading in degrees (0-360)
    #[wasm_bindgen]
    pub fn calculate_heading(&mut self, x: i32, y: i32) -> f64 {
        // Convert magnetometer readings to heading using atan2
        let mut heading = (y as f64).atan2(x as f64) * 180.0 / PI;
        
        // Normalize to 0-360 range
        if heading < 0.0 {
            heading += 360.0;
        }
        
        // Apply exponential smoothing with wraparound handling
        let mut diff = heading - self.last_heading;
        
        // Handle 360/0 boundary
        if diff > 180.0 {
            diff -= 360.0;
        } else if diff < -180.0 {
            diff += 360.0;
        }
        
        // Dead zone to prevent jitter
        const DEAD_ZONE: f64 = 0.5;
        if diff.abs() < DEAD_ZONE {
            return self.last_heading;
        }
        
        self.last_heading += diff * self.smoothing_factor;
        
        // Normalize result
        if self.last_heading < 0.0 {
            self.last_heading += 360.0;
        } else if self.last_heading >= 360.0 {
            self.last_heading -= 360.0;
        }
        
        self.last_heading
    }

    /// Reset the smoothing (useful when restarting)
    #[wasm_bindgen]
    pub fn reset(&mut self) {
        self.last_heading = 0.0;
    }

    /// Set smoothing factor (0.0 = no smoothing, 1.0 = instant change)
    #[wasm_bindgen]
    pub fn set_smoothing(&mut self, factor: f64) {
        self.smoothing_factor = factor.clamp(0.0, 1.0);
    }
}

/// Calculate distance between two GPS coordinates using Haversine formula
/// Returns distance in meters
#[wasm_bindgen]
pub fn calculate_distance(lat1: f64, lon1: f64, lat2: f64, lon2: f64) -> f64 {
    const EARTH_RADIUS: f64 = 6371000.0; // meters
    
    let lat1_rad = lat1.to_radians();
    let lat2_rad = lat2.to_radians();
    let delta_lat = (lat2 - lat1).to_radians();
    let delta_lon = (lon2 - lon1).to_radians();
    
    let a = (delta_lat / 2.0).sin().powi(2) +
            lat1_rad.cos() * lat2_rad.cos() *
            (delta_lon / 2.0).sin().powi(2);
    
    let c = 2.0 * a.sqrt().atan2((1.0 - a).sqrt());
    
    EARTH_RADIUS * c
}

/// Calculate bearing from current position to target position
/// Returns bearing in degrees (0-360)
#[wasm_bindgen]
pub fn calculate_bearing(lat1: f64, lon1: f64, lat2: f64, lon2: f64) -> f64 {
    let lat1_rad = lat1.to_radians();
    let lat2_rad = lat2.to_radians();
    let delta_lon = (lon2 - lon1).to_radians();
    
    let y = delta_lon.sin() * lat2_rad.cos();
    let x = lat1_rad.cos() * lat2_rad.sin() -
            lat1_rad.sin() * lat2_rad.cos() * delta_lon.cos();
    
    let mut bearing = y.atan2(x).to_degrees();
    
    // Normalize to 0-360
    bearing = (bearing + 360.0) % 360.0;
    
    bearing
}

/// Get compass direction as string (N, NE, E, SE, S, SW, W, NW)
#[wasm_bindgen]
pub fn get_direction(heading: f64) -> String {
    let normalized = ((heading % 360.0) + 360.0) % 360.0;
    
    match normalized {
        h if h >= 337.5 || h < 22.5 => "N".to_string(),
        h if h >= 22.5 && h < 67.5 => "NE".to_string(),
        h if h >= 67.5 && h < 112.5 => "E".to_string(),
        h if h >= 112.5 && h < 157.5 => "SE".to_string(),
        h if h >= 157.5 && h < 202.5 => "S".to_string(),
        h if h >= 202.5 && h < 247.5 => "SW".to_string(),
        h if h >= 247.5 && h < 292.5 => "W".to_string(),
        h if h >= 292.5 && h < 337.5 => "NW".to_string(),
        _ => "?".to_string(),
    }
}

/// Format distance for display (converts to km if > 1000m)
#[wasm_bindgen]
pub fn format_distance(meters: f64) -> String {
    if meters >= 1000.0 {
        format!("{:.2} km", meters / 1000.0)
    } else {
        format!("{:.0} m", meters)
    }
}

/// Calculate relative bearing (difference between heading and bearing to target)
/// Returns angle in degrees (-180 to +180)
#[wasm_bindgen]
pub fn calculate_relative_bearing(current_heading: f64, target_bearing: f64) -> f64 {
    let mut diff = target_bearing - current_heading;
    
    // Normalize to -180 to +180
    while diff > 180.0 {
        diff -= 360.0;
    }
    while diff < -180.0 {
        diff += 360.0;
    }
    
    diff
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_calculate_heading() {
        let mut calc = CompassCalculator::new();
        
        // Test north (0°)
        let heading = calc.calculate_heading(100, 0);
        assert!((heading - 0.0).abs() < 1.0);
        
        // Test east (90°)
        calc.reset();
        let heading = calc.calculate_heading(0, 100);
        assert!((heading - 90.0).abs() < 1.0);
    }

    #[test]
    fn test_distance_calculation() {
        // Distance from Cairo to Alexandria (approx 179km)
        let distance = calculate_distance(30.0444, 31.2357, 31.2001, 29.9187);
        assert!((distance - 179000.0).abs() < 5000.0); // Within 5km
    }

    #[test]
    fn test_direction() {
        assert_eq!(get_direction(0.0), "N");
        assert_eq!(get_direction(45.0), "NE");
        assert_eq!(get_direction(90.0), "E");
        assert_eq!(get_direction(180.0), "S");
        assert_eq!(get_direction(270.0), "W");
    }
}
