class String:
    class Particle:
        def __init__(self, previous_position, position, fixed):
            self.previousPosition = [
                float(previous_position[0]), float(previous_position[1])]
            self.position = [float(position[0]), float(position[1])]
            self.fixed = fixed

    def __init__(self, x1, y1, x2, y2, n_particles):
        self.fixPoint = [float(x1), float(y1)]
        self.endPoint = [float(x2), float(y2)]
        self._particles = []

        for i in range(n_particles):
            w = i / (n_particles - 1)
            pos = [w * self.endPoint[0] + (1 - w) * self.fixPoint[0],
                   w * self.endPoint[1] + (1 - w) * self.fixPoint[1]]
            p = self.Particle(pos, pos, i == 0)
            self._particles.append(p)

        self.calculateDesiredDistance()
        self._timestep = 0.01
        self._constraintsIterations = 35
        self._gravity = [0.0, 9.81]

    def step(self):
        self.verletIntegration()
        self.enforceConstraintsJakobsen()

    def addParticle(self, x, y, fixed):
        self._particles.append(self.Particle(
            [float(x), float(y)], [float(x), float(y)], fixed))
        self.calculateDesiredDistance()

    def setTimestep(self, timestep):
        self._timestep = timestep

    def setConstraintIterations(self, iterations):
        self._constraintsIterations = iterations

    def setSegmentLength(self, length):
        self._desiredDistance = length

    def getTimestep(self):
        return self._timestep

    def getConstraintIterations(self):
        return self._constraintsIterations

    def getSegmentLength(self):
        return self._desiredDistance

    def getParticles(self):
        positions = []
        for p in self._particles:
            velocity = [(p.position[0] - p.previousPosition[0]) / self._timestep,
                        (p.position[1] - p.previousPosition[1]) / self._timestep]
            positions.append(
                [p.position[0], p.position[1], velocity[0], velocity[1]])
        return positions

    def verletIntegration(self):
        for i, p in enumerate(self._particles):
            if not p.fixed:
                positionCopy = p.position.copy()
                self._particles[i].position = [2 * p.position[0] - p.previousPosition[0] + (self._timestep ** 2) * self._gravity[0],
                                               2 * p.position[1] - p.previousPosition[1] + (self._timestep ** 2) * self._gravity[1]]
                self._particles[i].previousPosition = positionCopy

    def enforceConstraintsJakobsen(self):
        for iteration in range(self._constraintsIterations):
            for i in range(1, len(self._particles)):
                p1 = self._particles[i - 1]
                p2 = self._particles[i]

                deltaDistance = ((p2.position[0] - p1.position[0]) ** 2 + (
                    p2.position[1] - p1.position[1]) ** 2) ** 0.5 - self._desiredDistance
                direction = [(p2.position[0] - p1.position[0]) / ((p2.position[0] - p1.position[0]) ** 2 + (p2.position[1] - p1.position[1]) ** 2) ** 0.5,
                             (p2.position[1] - p1.position[1]) / ((p2.position[0] - p1.position[0]) ** 2 + (p2.position[1] - p1.position[1]) ** 2) ** 0.5]

                if p1.fixed:
                    p2.position[0] -= direction[0] * deltaDistance
                    p2.position[1] -= direction[1] * deltaDistance
                elif p2.fixed:
                    p1.position[0] += direction[0] * deltaDistance
                    p1.position[1] += direction[1] * deltaDistance
                else:
                    p1.position[0] += direction[0] * 0.5 * deltaDistance
                    p1.position[1] += direction[1] * 0.5 * deltaDistance
                    p2.position[0] -= direction[0] * 0.5 * deltaDistance
                    p2.position[1] -= direction[1] * 0.5 * deltaDistance

                self._particles[i - 1] = p1
                self._particles[i] = p2

    def calculateDesiredDistance(self):
        if len(self._particles) < 2:
            return

        totalDistance = 0
        for i in range(1, len(self._particles)):
            p1 = self._particles[i - 1]
            p2 = self._particles[i]
            totalDistance += ((p2.position[0] - p1.position[0]) **
                              2 + (p2.position[1] - p1.position[1]) ** 2) ** 0.5

        self._desiredDistance = totalDistance / (len(self._particles) - 1)


# # import numpy as np


# class String:
#     class Particle:
#         def __init__(self, previous_position, position, fixed):
#             self.previousPosition = np.array(previous_position, dtype=float)
#             self.position = np.array(position, dtype=float)
#             self.fixed = fixed

#     def __init__(self, x1, y1, x2, y2, n_particles):
#         self.fixPoint = np.array([x1, y1], dtype=float)
#         self.endPoint = np.array([x2, y2], dtype=float)
#         self._particles = []

#         for i in range(n_particles):
#             w = i / (n_particles - 1)
#             pos = w * self.endPoint + (1 - w) * self.fixPoint
#             p = self.Particle(pos, pos, i == 0)
#             self._particles.append(p)

#         self.calculateDesiredDistance()
#         self._timestep = 0.01
#         self._constraintsIterations = 35
#         self._gravity = np.array([0, 9.81], dtype=float)

#     def step(self):
#         self.verletIntegration()
#         self.enforceConstraintsJakobsen()

#     def addParticle(self, x, y, fixed):
#         self._particles.append(self.Particle([x, y], [x, y], fixed))
#         self.calculateDesiredDistance()

#     def setTimestep(self, timestep):
#         self._timestep = timestep

#     def setConstraintIterations(self, iterations):
#         self._constraintsIterations = iterations

#     def setSegmentLength(self, length):
#         self._desiredDistance = length

#     def getTimestep(self):
#         return self._timestep

#     def getConstraintIterations(self):
#         return self._constraintsIterations

#     def getSegmentLength(self):
#         return self._desiredDistance

#     def getParticles(self):
#         positions = np.zeros((len(self._particles), 4), dtype=np.float32)
#         for i, p in enumerate(self._particles):
#             velocity = (p.position - p.previousPosition) / self._timestep
#             positions[i] = [p.position[0],
#                             p.position[1], velocity[0], velocity[1]]
#         return positions

#     def verletIntegration(self):
#         for i, p in enumerate(self._particles):
#             if not p.fixed:
#                 positionCopy = p.position.copy()
#                 self._particles[i].position = 2 * p.position - \
#                     p.previousPosition + (self._timestep ** 2) * self._gravity
#                 self._particles[i].previousPosition = positionCopy

#     def enforceConstraintsJakobsen(self):
#         for iteration in range(self._constraintsIterations):
#             for i in range(1, len(self._particles)):
#                 p1 = self._particles[i - 1]
#                 p2 = self._particles[i]

#                 deltaDistance = np.linalg.norm(
#                     p2.position - p1.position) - self._desiredDistance
#                 direction = (p2.position - p1.position) / \
#                     np.linalg.norm(p2.position - p1.position)

#                 if p1.fixed:
#                     p2.position -= direction * deltaDistance
#                 elif p2.fixed:
#                     p1.position += direction * deltaDistance
#                 else:
#                     p1.position += direction * 0.5 * deltaDistance
#                     p2.position -= direction * 0.5 * deltaDistance

#                 self._particles[i - 1] = p1
#                 self._particles[i] = p2

#     def calculateDesiredDistance(self):
#         if len(self._particles) < 2:
#             return

#         totalDistance = 0
#         for i in range(1, len(self._particles)):
#             p1 = self._particles[i - 1]
#             p2 = self._particles[i]
#             totalDistance += np.linalg.norm(p2.position - p1.position)

#         self._desiredDistance = totalDistance / (len(self._particles) - 1)
