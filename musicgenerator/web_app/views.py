from django.shortcuts import render
from django.core import serializers
import scalable_music_generator as scalable

# Create your views here.
def index(request):
    return render(request,'index.html')
def aboutus(request):
    return render(request, 'aboutus.html')
def python_query(request, some_data):

    music_generator=scalable.Scalable_Midi_Generator(some_data)
    

    data = serializers.serialize('json', some_data)
    return HttpResponse(data, mimetype='application/json')
    #some_data=request.GET.get('some_data')
    #return some_data
