#!/bin/bash

# Simple script to generate Doxygen documentation locally
# This is a simplified version for local development

echo "Generating Doxygen documentation..."
cd "$(dirname "$0")"
mkdir -p static/doxygen

# Update Doxyfile to remove obsolete tags if needed
doxygen -u Doxyfile 2>/dev/null

# Generate the documentation
doxygen Doxyfile

if [ $? -eq 0 ]; then
    echo "Doxygen documentation generated successfully in static/doxygen/"
    echo "To build the Docusaurus site, run: npm run build"
    echo "To start the development server, run: npm run start"
else
    echo "Error: Failed to generate Doxygen documentation."
    exit 1
fi
