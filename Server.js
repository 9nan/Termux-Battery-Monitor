/**
 * Copyright (c) 2025 itznan
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// Import required modules
const express = require("express");
const cors = require("cors");

// Define constants
const PORT = 21132;

// Create an Express application
const app = express();

// Enable CORS and JSON parsing
app.use(cors());
app.use(express.json());

// Define data storage for battery data
let batteryData = {
  percentage: "Unknown",
  updated_at: "Never"
};

// Flag to track the first connection
let isFirstConnection = true;

/**
 * API route to receive battery data from phone
 *
 * @route POST /update-battery
 * @param {object} req.body - Request body containing battery data
 * @param {number} req.body.percentage - Battery percentage
 * @returns {object} Response object
 */
app.post("/update-battery", (req, res) => {
  const { percentage } = req.body;
  if (percentage !== undefined) {
    batteryData = {
      percentage: `${percentage}`,
      updated_at: new Date().toLocaleString()
    };
    return res.status(204).send(); // No response content (silent success)
  }
  res.status(400).json({ error: "Invalid data" }); // Only sends response on error
});

/**
 * API route to get battery percentage
 *
 * @route GET /battery
 * @returns {object} Response object containing battery data
 */
app.get("/battery", (req, res) => {
  res.json(batteryData);
});

/**
 * API route to check if the server is running
 *
 * @route GET /ping
 * @returns {object} Response object
 */
app.get("/ping", (req, res) => {
  if (isFirstConnection) {
    isFirstConnection = false;
    console.log("Phone connected to server.");
  }
  res.sendStatus(204); // Silent response
});

// Start the server
app.listen(PORT, () => {
  console.log(`Server running on http://localhost:${PORT}`);
});
