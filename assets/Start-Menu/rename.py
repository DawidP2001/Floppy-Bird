"""
Name: rename.py
Description: This module provides functionality to rename files located in the sequence directory.
It renames file from FrameXXXX to X format
"""

import os

# Get the directory where this script is located
script_dir = os.path.dirname(os.path.abspath(__file__))

# Define the sequence directory path (in the same directory as this script)
sequence_dir = os.path.join(script_dir, "Sequence")

# Check if directory exists
if not os.path.exists(sequence_dir):
    print(f"Error: Directory '{sequence_dir}' not found!")
    exit(1)

# Get all files in the sequence directory
files = os.listdir(sequence_dir)

# Filter and rename files
for filename in files:
    # Check if filename starts with 'frame' (case insensitive)
    if filename.lower().startswith('frame'):
        # Extract the number part (e.g., 'frame0000' -> '0000')
        num_str = filename[5:]  # Remove 'frame' prefix
        
        # Get file extension if any
        name_parts = num_str.split('.')
        if len(name_parts) > 1:
            number = name_parts[0]
            extension = '.' + '.'.join(name_parts[1:])
        else:
            number = num_str
            extension = ''
        
        # Convert to integer to remove leading zeros, then back to string
        try:
            new_name = str(int(number)) + extension
            
            old_path = os.path.join(sequence_dir, filename)
            new_path = os.path.join(sequence_dir, new_name)
            
            # Rename the file
            os.rename(old_path, new_path)
            print(f"Renamed: {filename} -> {new_name}")
        except ValueError:
            print(f"Skipped: {filename} (couldn't parse number)")

print("Renaming complete!")