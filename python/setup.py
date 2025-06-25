# from distutils.core import setup
import setuptools

with open("README.md", "r") as fh:
    long_description = fh.read()

setuptools.setup(
    name='sm_4rel4in',
    packages=setuptools.find_packages(),
    version='1.0.6',
    license='MIT',
    description='Library for Sequent Microsystems Four Relays four HV Inputs 8-Layer Stackable HAT for Raspberry Pi',
    author='Sequent Microsystems',
    author_email='olcitu@gmail.com',
    url='https://sequentmicrosystems.com',
    keywords=['industrial', 'raspberry', 'power', 'optoisolated', 'relay', 'pulse count', 'pwm read',
              'quadrature encoders', 'frequency read', 'digital input', 'AC/DC input'],
    install_requires=[
        "smbus2",
    ],
    classifiers=[
        'Development Status :: 4 - Beta',
        # Chose either "3 - Alpha", "4 - Beta" or "5 - Production/Stable" as the current state of your package
        'Intended Audience :: Developers',
        'Topic :: Software Development :: Build Tools',
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.4',
        'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
    ],
)
