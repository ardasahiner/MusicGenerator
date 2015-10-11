from django.shortcuts import render
from django.utils import simplejson

# Create your views here.
def index(request):
    return render(request,'index.html')
def aboutus(request):
    return render(request, 'aboutus.html')
def python_query(request, some_data):

    return HttpResponse(simplejson.dumps(some_data), mimetype='application/json')
    #some_data=request.GET.get('some_data')
    #return some_data
