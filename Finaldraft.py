# The GBplaces.csv file was used to produce a visual plot of the population, longitude, latitude with labels for the name of town/city.

#imports the matplotlib and random libraries which will be used to produce graphs, with random used to space out the labels
import matplotlib.pyplot as plt
import random

#opens the GBplaces.csv file and reads it
readFile=open('GBplaces.csv','r')


#empty arrays created for the different columns in the csv file
place=[]
types=[]
population=[]
latitude=[]
longitude=[]

#all the columns have been split and separated into different arrays, with the first row in the file (headers) ignored
for line in readFile:
    if line[0] != '%':
        
        splitUp=line.split(',')
        place.append(splitUp[0])
        types.append(splitUp[1])
        population.append(int(splitUp[2]))
        latitude.append(splitUp[3])
        longitude.append(splitUp[4])
#closes csv file
readFile.close

#opens up the GBmap image
im = plt.imread('GBmap.jpg')

#assigns an origin and set of axes to the image which will be in the background. The axes stretch the image. The origin is centred in the bottom left corner
extent = (-5.2, 2.7, 50, 57.8)
plt.imshow(im, origin='upper', extent=extent,aspect='auto')

#sets the scale of axes to be used for the data points for population. Axes are labelled too with longitude and latitude
plt.xlim(-4.9,2.1)
plt.ylim(50,57.8)
plt.xlabel('Longitude (°)',fontsize=8)
plt.ylabel('Latitude (°)',fontsize=8)

#converts latitude and longitude arrays to to floats (y an x respectively)
y=[float(i) for i in latitude]
x=[float(i) for i in longitude]
#creating an array of labels for points out of the names in 'place'. This step is for convenience
pointlabels=place

#for all the labels, the corresponding longitude and latitude co ordinates have been converted to flaots
for i, label in enumerate(pointlabels): 

#duplicate arrays created for the longitude and latitude arrays from before
    valueX=x[i]
    valueY=y[i]

#random numbers generated to be used later to minimise overlap of labels
    randomX=random.uniform(-0.001,0.001)
    randomY=random.uniform(-0.08,0.08)
#a for loop produced. Magnitude of difference between longitude values found and if less than 0.02, then random number (randomX) is added on to offset the labels and minimise overlap.  Same is repeated for the y (latitude) values, using randomY to offset them if less than 0.2.
    for g in range(i-1):
          
        getX = float(x[g])
        getY = float(y[g])
        
        if(abs(getX-valueX<0.02)):
          
            valueX = valueX+randomX
        if(abs(getY-valueY<0.1)):
       
            valueY=valueY+randomY
            break
#the text labels are added to the visual plot to each point, but slightly offset from the longitude'latitu
    plt.text(valueX,valueY,pointlabels[i],fontsize=1.3,color='yellow')
#producing an array where each city is replaced with a 'green' string while towns have 'orange' strings. Will be used to determine colour of markers depending on whether they are cities or towns
types_binary=[]
for item in types:
    if item=='City':
        item='green'
    else:
        item='orange'
    types_binary.append(item)
#scaling down the values of all values of population to create an array which will be used to determine the size of markers, where larger markers indicates a larger population
popscaled=[i/(6*10e3) for i in population]
   
#plotting the points for each location, corresponding to a green/orange colour
plt.scatter(x,y,s=popscaled,lw=0.2,c=types_binary)  
#a title for the visual plot has been created and placed at the top
plt.title('A visual plot for the top 100 most populous places in the UK')

#Producing a legend for whether a point corresponds to a town or city. Placed in the upper right area
plt.plot(0,0,c='green',label='City')
plt.plot(0,0,c='orange',label='Town')
plt.legend(loc='upper right')

#Image file is exported as a JPEG
plt.savefig('GBplaces_visualised.jpg',dpi=1000, bbox_inches='tight')