# Create and navigate to build directory
mkdir -p build
cd build

# Run CMake to configure the project
cmake ..

# Build the project
make 

# Check if the build was successful
if [ $? -eq 0 ]; then
    echo "Build succeeded."
    # Run tests
    ./tests/testing
else
    echo Build failed.
fi