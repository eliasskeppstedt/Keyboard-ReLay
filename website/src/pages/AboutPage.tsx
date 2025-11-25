
import React from 'react';

export const AboutPage: React.FC = () => {
    return (
        <div style={{ display: 'flex', flexDirection: 'column', alignItems: 'center', color: '#EDEAE3', marginTop: '60px' }}>
            <h1 style={{ fontSize: '2.5rem', marginBottom: '20px' }}>About</h1>
            <div className="panel-bg-box" style={{ maxWidth: '700px', lineHeight: '1.6' }}>
                <p>
                    NOTE! project is not done and many features are not implemented.
                </p>
                <p>
                    Keyboard ReLay is a tool that simplifies modifications to your keyboard.
                    I got into costum keyboards a while back and eventually bougth the Voyager from
                    <a
                        href="https://www.zsa.io/"
                        target="_blank"
                        rel="noopener noreferrer"
                        style={{
                            color: '#9f9cff',
                            textDecoration: 'none',
                            marginLeft: '4px'
                        }}
                    >
                        ZSA
                    </a>.
                    It is a split keyboard where you can costumize everything from rebinding keys to
                    adding multiple layers. This makes it possible to reuse the same physical key
                    for multiple keys which allows your hands to move less while typing. This
                    strongly takes advantage of your touch typing skills, so in order to fully take advantage
                    of the tool i recomend you to get familiar with touchtypign and maybe reach avarage speed
                    (~40wpm).
                    <br /><br />
                    As i said i have been using the Voyager for a while and i really enjoy it, and one
                    of the things i like the most is their intuitive layout maker 
                    <a
                        href="https://configure.zsa.io/home"
                        target="_blank"
                        rel="noopener noreferrer"
                        style={{
                            color: '#9f9cff',
                            textDecoration: 'none',
                            marginLeft: '4px'
                        }}
                    >
                        Oryx
                    </a>. My tool is much
                    inpired by it, and with this ive tried to make it possible for ordinary keyboard 
                    users to feel the joy of this type of writing!
                </p>
                
                <div style={{ marginTop: '30px', borderTop: '1px solid #2A2A28', paddingTop: '20px' }}>
                    <h3 style={{ fontSize: '1.2rem', marginBottom: '10px', color: '#EDEAE3' }}>Project & Contact</h3>
                    <p>
                        This project is a hobby project and thus im not very active at it. Here are my github page to see the project status:
                        <br />
                        <a 
                            href="https://github.com/eliasskeppstedt/Keyboard-ReLay" 
                            target="_blank" 
                            rel="noopener noreferrer"
                            style={{ color: '#9f9cff', textDecoration: 'none', fontWeight: 'bold', display: 'inline-block', marginTop: '8px' }}
                        >
                            https://github.com/eliasskeppstedt/Keyboard-ReLay
                        </a>
                    </p>
                </div>
            </div>
        </div>
    );
};
