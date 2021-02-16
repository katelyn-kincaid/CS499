## Self Assessment
Throughout my time in the Computer Science program at SNHU, I’ve had to complete several projects that have pushed me to become more capable in the field. Two of those projects have been showcased via the enhancements you can see more about below. 

I have had to work with python, C++, and java primarily. I’ve had to learn to interpret assembly code. I’ve had to learn various Linux commands. Throughout my academic journey, I’ve questioned whether this field was for me. Some aspects, namely interpreting assembly code, seemed far too difficult to get through. Then, when I am presented with a particularly challenging project, I found myself to be so entranced with what I was doing that hours would pass by without me noticing. I was so hyper-focused on the task at hand that I would lose track of time. 

I’ve made it to the end of this academic chapter. I’ve passed every test and trial thrown at me. To get to this stage, I’ve noticed that there are several character attributes that are necessary to success. First, you need to be able to collaborate in a team environment This includes working with peers and working with mentors. The academic environment allows us to utilize our instructors as our mentors as we learn and our fellow students as our peers. CS310 was my first experience with collaborating in a team environment, though other courses have mentioned the team dynamic. I’m aware that I do not know nearly enough in this field, so I understand that I need to work with a team to fill in all the gaps of knowledge. What I know well could be the gap of knowledge for someone else. 

Second, you need to be able to communicate to stakeholders. To excel at this attribute, you need to be able to explain what you’re doing so that someone without coding knowledge would be able to understand. This is important since you may end up working for clients that have no background in computer science, so you need to be able to explain a project in simple terms. This attribute was subtly addressed over numerous courses. In the beginning, it began with classes demanding for pseudo code. This was the first step in being able to turn plain language into coding language. It was enough to get you in the right mindset to begin a project. CS320 is what truly polished this attribute, as it required me to reflect on the actions I had taken in the code and explain everything as if I were talking to someone with no knowledge of coding. I had to translate every action I took into plain, simple language.

Third, competency in data structures and algorithms. Understanding this attribute was a key component in numerous courses. However, I felt as if I was truly able to demonstrate my competency in this attribute with my final project submission for CS350. For this project, I was tasked with creating a device that would collect data and conduct various outputs at specific instances. For example, it would only record data if it were during the day. I had to create an algorithm that accomplished this, and I was successful in it. Additionally, I had to ensure the different colored LED lights would only light up during specified conditions, which I did with another algorithm. The data that was collected had to be sent to a JSON file, which it was. Ultimately, I chose to highlight this project for Enhancement Two and Three. 

Fourth, competency in software engineering and databases. Understanding your way around a database was a common theme in many of the classes taken for computer science. CS340 was when MongoDB was initially introduced. I used what I learned from that course to enhance a separate and unrelated project by integrating MongoDB. That knowledge made Enhancement Three a complete success, which should be evidence enough that I know my way around a database. 

Fifth, understanding of security. CS405 was the first course I took that truly focused on security. It was later addressed further in CS410, but with a different focus on reverse engineering. CS405 taught me to be mindful of what I code, as it could result in unforeseen consequences like a buffer overflow. That class also taught me of new ways I could use to validate my code that could hopefully prevent any unintentional vulnerabilities. Thanks to that class, I now have a better understanding on security and how to ensure my code is as secure as possible. 

The artifacts I chose to highlight were “table” from CS330 and “weather” from CS350. “table” was the culmination of my introduction to OpenGL. I not only learned how to create 2D and 3D objects, but I also learned how to attach a texture, introduce a light source, and implement movement. This project was chosen for Enhancement One: Software Design and Engineering because it focused on engineering a 3D object. I wanted to enhance it by adding more complexity in the form of a separate and unique object. 

Ultimately, I was able to create two separate and unique objects that could move independently of one another. However, I was not able to maintain my use of textures and mouse movements, making this enhancement only a partial success. To pull it off, I had to demonstrate my ability at software engineering. Though I couldn’t maintain the use the textures and mouse movements, I was successful in the implementation of two separate and unique objects that were still able to move independently from one another. 

The final project for CS350, or “weather”, was what I selected for Enhancement Two: Algorithms and Data Structures and Enhancement Three: Databases. For “weather”, I was tasked with putting together a physical device that was able to record various inputs like temperature, humidity, and light levels. I then had to set up a variety of algorithms that ensured only certain inputs would be recorded, and at specified times, and sent to a JSON file. To enhance this, I decided to increase the amount of data that would be recorded and sent to a JSON file while removing the impediment of only recording during a certain time. I also wanted to integrate MongoDB. 

I was able to accomplish this, which showcased my competency in data structures and algorithms as well as databases. Instead of the device just recording temperature and humidity data, I had it collect light levels and record exactly when the data was recorded. I wanted to expand the kind of data that was collected to make it a more useful application. It’s fine to know the temperature and humidity levels, but what good is that information if you don’t know when it was collected? That kind of questioning also demonstrates my ability to understand stakeholders. To inject that level of logic is vital in ensuring you can make your stakeholders happy. 

 I wanted to integrate MongoDB so that I could have a different way to view the data. This added a redundancy in the project while making it easier to validate that the data was collected correctly so that it would be viewable. It gave me a much easier and compact way to view the data. It also clearly demonstrated my ability to work with databases. 

### Code Review

To view my code review, go [here](https://youtu.be/0712i8ezSaw). 

### Refinement Predictions

If I am successful with my enhancements, then I will be able to demonstrate extreme attention to detail, the ability to adapt and overcome, and resourcefulness. In project Table, I am starting with an existing project that consists of over 600 lines of code. To achieve my enhancements, I must comb through every line and add in new lines of code to place a new object seamlessly on the existing table. Doing all that successfully no doubt proves that I have developed a great attention to detail. In project Weather, I am posing the realistic expectation of adjusting an existing project to better fit the new needs of myself or a client. Doing so may require me to look at a project I spent countless time on with fresh eyes. To pull that off shows that can adapt to new challenges and overcome them through resourcefulness. 

My cause for concern in project Table lies with creating an entirely new shape to add to the project. When I was taking that course, I noticed how the shape of the light copied the shape of the object it was meant to illuminate. I believe I caused that to happen because I was not paying enough attention to the details, so I am optimistic that I can get the results I want this time.  In project Weather, creating JSON files was the biggest issue I had. I believe the bulk of my work will be spent tweaking that bit of code so that the JSON file records the exact kind of data I want. I don’t foresee any issues with implementing Mongo DB into project Weather, but it’s still early. Other than a couple of courses requiring the creation of JSON files, I don’t have much training in that area. This I feel will be my biggest weakness, but it’s one I intend to overcome. 

### Enhancement One: Software Design and Engineering

For the category of software design and engineering, I will be adding to the complexity of the final project I submitted for CS-330 Computational Graphics and Visualization. For that project, I was tasked with creating a virtual object based on an existing item of furniture I owned. For ease, it will be referred to as “table,” as that is the object I chose to replicate virtually. To enhance the existing code, my plan will be to add a different object on top of the table. During this project I noticed a distinct difficulty with implementing different shapes within the same file. I’d like to use this project as motivation to overcome this.   

The pseudocode can be viewed below. 

```markdown
//the mentioned project has 664 lines of code, and changes will need 
//to be made extensively throughout. Pseudocode to follow will be 
//incredibly vague and can be applied in multiple spots
#define WINDOW_TITLE “Table” //re-naming to fit this project
Table position and scale parems
Table light and color
Object on table position and scale parems //new addition
Object on table light and color //new addition
Light position and scale
Table vertex shader source code
Global variables for the transform matrices
Table fragment shader source code
Object on table vertex shader source code //new addition
Global variables for the transform matrices //new addition
Object on table fragment shader source code //new addition
Lamp shader source code
Lamp fragment shader source code
Shader program
	Table vertex shader
	Table fragment shader
	Table shader program
	Delete vertex and fragment shaders once linked
	Object on table vertex shader //new addition
	Object on table fragment shader //new addition
	Object on table shader program //new addition
	Delete object on table vertex and fragment shaders once linked //new addition
	Lamp vertex shader
	Lamp fragment shader
	Lamp shader program
	Delete lamp shaders once linked
Draw the vertices for the table
Draw the vertices for the object //new addition
```
Now that the enhancements have been made, I can go back and reflect on this. I really did try to keep with the pseudocode above, but it ultimately proved to be unsuccessful. You can see how it turned out by looking at the RefinedTable file listed [here](https://github.com/katelyn-kincaid/CS499/tree/EnhancementOne). 

However, I was partially successful with the creation of a second, unique object. I was able to create a cube on top of my table, and both had independent movement of one another. I was not able to keep the texture, lighting, or mouse movement elements. I kept trying to further refine the project, but I could not get past this hurdle. 

### Enhancement Two: Algorithms and Data Structure

For the category of algorithms and data structures, I will be improving the efficiency of the final project I submitted in CS-350 Emerging Systems Architectures and Technologies. For that project I was tasked with creating a temperature and humidity data collecting device that would only record data during day-light hours. I will re-program that same weather data-collecting device to record not only the temperature and humidity levels, but to also record if it’s light or dark and the time of day. I will then enhance the existing code that sends the collected data to a json file to include everything I listed. 

The pseudocode can be viewed below. 

```markdown
#starting at the JSON portion of code
#get sensor value (i.e., indications of it being light or dark in the room)
# what will read light in the room
lightsensor_value = grovepi.analogRead(light_sensor) 
#create a formula that will determine if the room is light or dark
resistance = (float)(1023 – lightsensor_value) * 10 / lightsensor_value 
#note, the above was the formula used for the previous assignment, will have to re-calculate
#initiate if/else statements
If resistance is less than the threshold
Data will be sent to a json file #record temp, humidity, time of day, and light levels
Set frequency to 10 minutes 
Else
Print no data collected at TIME
```

With this enhancement done as well, I can confidently say that I was able to accomplish everything I set out to for this projet. I wanted to ensure temperature, humidity, time of day, and light levels would be recorded and sent to a JSON file. The only thing I strayed on would be changing the frequency of data collection to 10 minutes. You can find the pre and post enhancement refinement [here](https://github.com/katelyn-kincaid/CS499/tree/EnhancementTwo).  

### Enhancement Three: Databases

In the databases category, I will attempt to create a MongoDB interface with the same weather project used in the previous category from CS-350 Emerging Systems Architectures and Technologies. While I’m in the terminal of my raspberry Pi setup, I will initiate Mongo DB. In its current state, the weather project saves certain weather data to a json file to be viewed on an HTML5 canvas. My intent will be to view the same json files through Mongo DB. Doing so will make it quicker and easier for me to access the weather data. Adding this feature to my existing code was made possible by taking CS-340 Advanced Programming Concepts. 

The pseudocode can be viewed below. 

```markdown
#While in the terminal
../startMongod.sh
View available databases
View files
Open json file that holds weather data
```

With this enhancement done, I can say that I was able to accomplish exactly what I set out to do. However, it was not done as easily as the pseduocode would suggest. First I had to download MongoDB to my Raspberry Pi set up. Next I had to import the JSON file. After that, I used different commands than listed in the pseudocode. Instead of using ../startMongod.sh, I just had to use mongo. Then I showed the databases and files, and eventually the specific file I was looking for. There isn't exactly code, so to speak, for this enhancement. For others to view, I included the pre and post enhancement JSON files as well as the narrative I drafted upon completion. You can find the full list of files pertaining to Enhancement Three [here](https://github.com/katelyn-kincaid/CS499/tree/EnhancementThree).

### Markdown

Markdown is a lightweight and easy-to-use syntax for styling your writing. It includes conventions for

```markdown
Syntax highlighted code block

# Header 1
## Header 2
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and ![Image](src)
```

For more details see [GitHub Flavored Markdown](https://guides.github.com/features/mastering-markdown/).

### Jekyll Themes

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/katelyn-kincaid/CS499/settings). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

### Support or Contact

Having trouble with Pages? Check out our [documentation](https://docs.github.com/categories/github-pages-basics/) or [contact support](https://github.com/contact) and we’ll help you sort it out.
