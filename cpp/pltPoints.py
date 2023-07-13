import matplotlib.pyplot as plt

# Define your 10 points as two lists, one for the x-coordinates and one for the y-coordinates
x = [2,3,4,5,6,7,8,9,10,11,12,13,19]
y = [3,4,5,5,6,6,7,8,8,9,10,10,15]

# Plot the points using the scatter function
plt.scatter(x, y)

# Add labels to the x- and y-axes
plt.xlabel('Num Servers')
plt.ylabel('runtime (s)')

# Add a title to the graph
plt.title('runtime for 1000 inputs on metric space with 20 points, and inputs of length 20')

# Display the graph
plt.show()