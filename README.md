# PSO-Group-Assignament
Use PSO to assign X students with subjects to X group.

# INTRODUCTION
We have members of a set that must be assigned to subsets optimizing a certain
function. 

To illustrate the problem we are going to consider students of a class that must be assigned
to subgroups for the practices. We consider np people, ng subgroups and na subjects. The
Table 1 shows an example of the selection of subjects by the students. In this example, np = 9
and na = 4, and we will consider 3 subgroups, ng = 3. An assignment is given by a vector
assignment of np components, each with a value between 1 and ng, where assignment [i] = j
indicates that student i is assigned to subgroup j. With the assignment shown in the last
column of the table, the number of students in each subgroup for each of the subjects
it is shown in the last rows, labeled with subgroupX (AED (2,3,2), AEC (3,2,1), PCD
(3,1,3) and ASO (2,2,2)), and the last row shows for each subject the difference between the
maximum and the minimum number of students in the subgroups. We see that the assignment in some
subjects is balanced, with the same number of students in each subgroup (ASO), but
in others there are differences between the number of students assigned to the subgroups. 
We want obtain all the possible assignments, which minimizes the imbalance.


