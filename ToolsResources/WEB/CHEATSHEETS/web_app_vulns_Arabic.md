[source](https://goo.gl/SYzCG9)

البوست دة #Part_23 من البوست الأول بتاع ازاي تبدأ...
البوست دة #Part_23 من البوست الأول بتاع ازاي تبدأ ال Career بتاعك في ال Cyber Security ?! الي عنده حد مهتم بالموضوع دة ياريت #يشير معاه البوست دة لانه هينفعه جداً ان شاء الله...زي ما الحمد لله الأجزاء الي فاتت نفعوا ناس كتير...
المفروض الجزء دة كنت هكمل ال Web Evasion Techniques بس لقيت ان فيه حاجة لازم اشرحها الأول الي هي ال Web Applications Vulnerabilities انا كنت شرحت قبل كدة في #Part_12 ال SQL-Injection بالتفصيل ف عشان اشرح ال Web Evasion Techniques لازم اشرح الاول باقي ال Vulnerabilities الي احنا بنقابلها سواء في ال Bug Bounties او في ال Penetration Testing العادي الي بنعمله طبعاً انا مش هقدر اقول كل ال Vulnerabilities الي خلقها ربنا بس هقول اكتر Vulnerabilities استخداماً او الناس بتلاقيها و الكلام دة من وجهة نظري يعني ممكن يكون فيه حاجات تانية مختلفة عني عشان محدش يقول ما انت مقولتش كذا و كذا.
شوية ملحوظات كدة قبل ما نبدأ ال Part :
1- اول حاجة ال Part دة مخصص لـ 3 انواع من الناس :
- ال Bug Hunters
- ال Web Application Pentesters
- ال Web Application Developers
2- ال Part دة اطول Part في ال Series كلها ف لو مش هتعرف تركز و انت بتقرأه خليه لوقت تكون مركز فيه لأن فيه حاجات ممكن مقدرش اقولها بالتفصيل ف هتاخدها و تـSearch عليها
3- لمدة 10 دقايق او ربع ساعة افصل كل الي حواليك لو مهتم فعلاً بالمجال دة.
--------------------------------------------------------------------------------
اول حاجة بس للناس الي لسة داخلة مجال ال Security جديدة و مصطلح Vulnerability جديد عليها : ال Vulnerability دي عبارة عن ثغرة يا اما موجودة في Logic ال Application يا اما موجودة في ال Source Code بتاع ال Application يا اما موجودة في ال Infrastructure بتاعت ال Application سواء ال Proxy ولا ال WAF ولا ال Web Server....الثغرة دة ليها حاجة اسمها Exploit
ايه معني ال Exploit ?! ال Exploit هو استغلال الثغرة الي بيسمحلي يا اما اخد Data مش بتاعتي او اشوف Data مش من حقي اشوفها او اخد Access علي مكان انا مش مسموحلي ادخله او ادخل Data مش من حقي ادخلها و غيرها من انواع ال Exploits المختلفة...دي كانت Hint سريعة للـ Cyber Security newbies عشان فيه ناس بتتلغبط ما بين ال Vulnerability و ال Exploit من ناحية المصطلح و المفهوم.
--------------------------------------------------------------------------------
الثغرة رقم #1 : الـ OS Command Injection
لما بيكون ال Developer مش عامل Filtering و Sanitization للـ User Input بيقدر ال User يدخل Commands اول ما تروح للـ Back-End يبتدي ال Interpreter يترجمها و يعرف انها Operating System Commands ف يروح ينفذها علي ال Server الي شايل ال Web Application....ثغرة ال OS Command Injection من اخطر الثغرات الي اي حد ممكن يكتشفها لأنها بتخلي ال User العادي الي ملوش اي صلاحيات انه ينفذ Commands علي ال Back-End و بكدة بيكون ليه ال Full Control علي ال Back-End الثغرة تقدر تخليه يقدر يتحكم في ال Application و ال Server و ال Database و اي حاجة موجودة في ال Back-end....طب مثال علي ال OS Command Injection :
https://pastebin.com/N2RrgsTN
--------------------------------------------------------------------------------
الثغرة رقم #2 : ال FPT او ال File Path Traversal
الثغرة دي بتكون Misconfiguration في ال Web Server بتسمح للـ User العادي انه يجرب يدخل علي Paths و Directories كتير في السيرفر لو عرف نوع ال Web Server ايه يعني مثلاً لو عرف انه Apache و ال Apache معروف انه Linux-Based فا ال Paths بتاعت ال Linux معروفة ف لو مثلاً عندك Web Application ممكن ال Attacker يجرب بقي يـ Access ال Paths بتاعت ال Linux عن طريق ال URL عن طريق انه يكتب اسم ال Path يعني مثلاً /tmp
من المعروف في ال Linux ان Folder ال tmp دة الي ال Operating System بيرمي فيها ال Temp Files عشان يستخدمها ف حاجة تانية او يخلص استخدامها
و ممكن مثلاً في ال Wordpress يقوم مجرب /wp-admin
و غيرها من ال Paths المشكلة الزيادة غير انه يقدر يـ Access ال Path انه يلاقي Files مهمة علي ال Paths دي و مش معمولها Privileges Handling يعني يقابل file و يكون Critical و في نفس الوقت واخد 777 يعني Full Access ف يقدر يعمله Download و يشوف لك ال Data الي فيه ماهو واخد Read و Write بقي.....
فا ال FPT من اهم الثغرات الي ال System Admins مبياخدوش بالهم منها و بيكونوا سايبين Files كتير مهمة في Paths مكشوفة و كمان بتكون ال Fies واخدة 777 من غير ال Admin ما ياخد باله ف بكدة اي حد هيقدر يـ Access ال Path دة هيعرف يعمل Download للـ Critical Files بتاعتك و يحصلك Sensitive Data Exposure كبير.
--------------------------------------------------------------------------------
الثغرة رقم #3 : ال XXE او ال XML external entity injection
دي تعتبر الوافد الجديد و الضيف الغير مرحب بيه في OWASP TOP 10 201 Report و لقيناها بقدرة قادر نطت في ال Report و كمان واخدة المركز الرابع في ال TOP 4 في ال Report.....طب الثغرة دي بتتكلم عن ايه ؟!
ال XXE بتقولك ان ال Attacker ممكن يعمل DDOS Attack او يعمل Execution لأي Code هو عايزه عن طريق ال XML Files و دة بيتم عن طريق انه يا اما يعمل Upload لل XML File بتاعه هو يا اما يقدر يـ Access ال XML Files الي عندك و يعدل في ال Content بتاعها.
طب امتي ال Application نقدر نقول عليه انه فيه ثغرة XXE ?!
و الله OWASP قالتلك عشان تعرف اذا كان ال Application بتاعك Vulnerable ولا لا روح اتأكد من ال Steps دي :
- قالتلك روح شوف لو ال XML Processors او ال SOAP عاملين Enable لل DTDs
طب معناه ايه الكلام دة ؟!
ال XML Processors دي وظيفتها انها بتقرأ ال XML Documents و بتحولها لصيغة تانية باقي ال Components بتاعت ال Application تقدر تقراها....ال SOAP دة الي هو Simple Object Access Protocol دة Protocol معمول عشان ال Applications تعرف تـ Communicate مع بعضها هتقولي ما ال HTTP بيعمل كدة ....هقولك لا فيه اختلاف...ال SOAP بيساعد ال Applications انها تـ Communicate مع بعضها لو ال Applications بتشتغل علي Operating Systems مختلفة و بتستخدم Technologies مختلفة و مكتوبة ب Programming Languages مختلفة.
ال DTDs دة الي هو Document Type Definitions هو الي بيحدد ال Structure و ال Elements و ال Attributes الي بتتكتب في ال XML Documents
- تاني حاجة OWASP قالتلك لو ال Application بتاعك بيستخدم SAML في انه عشان يـ Authorize و يـ Authenticate ال Processes الي بتتعمل عليه
ايه هو ال SAML دة .....ال SAML الي هو Security Assertion Markup Language دة Standard معمول عشان ال Exchange بتاع ال Authorization و ال Authentication بتاع ال Data الي ما بين ال Service Provider و ال Identity Provider الي يلاقي حاجة مش فاهمها يـ Search عليها ميفوتهاش دة لو مهتم بال Field دة..طب يا OWASP ايه الخطورة من ال SAML ?!
قالتلك ماهو ال SAML بيستخدم ال XML في ال identification و ممكن يبقي Vulnerable.
مثال للـ Attack الي ممكن ال Attacker يعمله عن طريق ال XXE :
https://pastebin.com/7Vi2tdf6
--------------------------------------------------------------------------------
الثغرة رقم #4 : HTTP PUT method is enabled
الناس الي عارف Web Development عارفة ان ال HTTP ليه Methods زي ال GET و ال POST و ال OPTIONS و ال TRACE , ال DELETE و ال PUT
فيه بقي Critical Vulnerability انك تلاقي ال PUT Method معمولها Enable في Protocol ال HTTP بتاع ال Application لان ال PUT بتسمحلك انك تقدر تعمل Upload لأي حاجة انت عايزها و تتخزن علي ال Web Server يعني لو لقيتها معمولها Enable تقدر تـ upload اي Malicious Content بقي Shell ولا Rootkit ولا Malware ولا اي حاجة.
--------------------------------------------------------------------------------الثغرة رقم #5 : SSI injection
ايه هي ال Server-Side Includes Injection و ايه مدي خطورتها ؟!
ايه هي ال SSI في الأول ؟!
ال Server-Side Includes ديعبارة عن شوية توجيهات و تعليمات كدة وظيفتها بكل بساطة انها بتضيف ال Dynamic Content علي ال HTML Page من غير ما تحتاج انك تـ Load ال Page كلها يعني مثلاً لو عايز اطبع التاريخ و الوقت و يروحوا يتجابوا من ال Server و يكونوا Real-Time مش معقولة كل ثانية هروح اعمل Refresh لل Page عشان تـ update الوقت ممكن اكتب السطر دة بس و يكون هو ال SSI :
https://www.htmlgoodies.com/…/34733…/SSI-Dates-and-Times.htm
الحتة دي بس الي هتكون Dynamic Content و هتروح تـ Load من ال Server مش ال Page كلها...
معني كدة ان ال SSI بتروح تجيب Data معينة من ال Server و ترجعلي بيها علي طول من غير طب ال SSI لو انا مش عامل Restriction و Validation علي ال Reponse بتاع ال SSI ممكن ال Attacker يعمل Injection لل SSI و يخليها تروح تنفذ اي Malicious Action علي ال Server زيها زي ال XSS بالظبط ممكن تروح تطلب من ال Sever حاجات زي ال Cookies و ال SessionID و غيرها و ترجعلي بيها .
--------------------------------------------------------------------------------
الثغرة رقم #6 : ال Cross-Site Scripting او ال XSS
ال XSS بكل بساطة و من غير تعقيد هي عبارة اني ببعت Script يتنفذ في ال Back-End و يا اما يرجعلي بال Result بتاعته يا اما مش عايزه يرجعلي المهم انه يتنفذ هناك و خلاص..
طب ايه هو ال Impact او التأثير الي ممكن ال XSS تعمله في ال Web Application او في ال Website ?!
- قالك اول حاجة ال XSS تقدر تعملها هي انها تعمل Execution لـ Active Content انها تقدر تنفذ Client-Side Scripts زي ال Javascripts و ال vbscripts بتاعت ال Visual basics و غيرها من ال Scripts
- تاني حاجة تقدر تـ Access ال Cookies بتاعتك و تسرق ال Session منك
- تالت حاجة تقدر تقرا ال Key-Strokes بتاعتك
- تقدر تقدر تبعت Data و تقدر تـ Access Forms
- تقدر تـ Exploit ال Vulnerabilities الموجودة في ال Browsers
- تقدر توصلك لل CSRF و ال Clickjacking
- تقدر تعمل Tunneling و Port Scanning علي ال Back-End
- تقدر تعمل Exploitation لل End-Points و تعمل Botnet
- ممكن تعمل حاجات كمان اكتر من كدة بكتير من خلال ال BeEF Framework
طب ايه هي انواع ال XSS ?!
عندنا رسمياً 3 انواع لل XSS :
- اول نوع اسمها Reflected XSS او ساعات بيسموه Non-Persistent XSS
- تاني نوع اسمها Stored XSS او ساعات بيسموه Persistent XSS
- تالت نوع سمها DOM XSS
ال Reflected XSS هي انك بتـ Inject ال Script بتاعك في ال URL و تبعت ال URL لل Victim و بعد كدة ال Victim يدوس علي ال Link ف يروح ال Browser بتاع ال Victim يروح ينفذ ال Script و يرجع بال Result بتاعته لل Attacker .
ال Victim لما يدوس علي ال Link دة بقي هيروح في ال Back-End ينفذ ال Function الي اسمها document.cookie ف هيرد ال Back-end بال Cookies بتاعت ال User بعد كدة يروح ينفذ باقي ال Script الي بتقوله ان ال Window Location يعني المكان الي هيطلع فيه ال cookies و تطبع هيكون علي ال Server بتاع ال Attacker الي انا حاطط ال IP بتاعه او ال URL في ال script برضو و بكدة يكون قدر ال Attacker انه ياخد ال Cookies بتاعت ال User عن طريق ال Reflected XSS Vulnerability .
ال Stored XSS او ال Persistent XSS دي من اخطر انواع ال XSS و اكتر الانواع الي بتسبب Damage لل Web Application....ال Hackers او ال Attackers عشان يعملوا Stored XSS بيدوروا علي ال Web Applications و ال Web Sites الي بتـ Share ال Content ما بين ال Users....طب ايه هي امثلة ال Web Sites دي :
- اول حاجة المنتديات و المواقع الي فيها Chat
- تاني حاجة ال Blogs
- تالت حاجة ال Web-Based Collaboration Tools زي Slack و غيره الي ال Content بيكون مشترك بين مجموعة من الناس
- رابع حاجة ال Systems بتاعت ال ERP وال CRM
ال Attacker او ال Hacker بيدور علي ال Fields الي بتكون Shared بين كل الناس و كل الناس بتشوفها زي ال Comments Fields يعني انت لما بتدخل علي Topic معين في اي Forum بتقدر تشوف ال Comments الي عليه....بيقوم ال Attacker بقي بيكتب Stored XSS Payload في ال comment دة لو ال website كان Vulnerable و بيقبل اي Data هيروح ال Stored XSS Payload دة يتخزن في ال Database و اي حد بعد كدة هيفتح ال Page الي فيها ال Comment دي هيحصل عنده Execution لل Stored XSS Payload و هيبعت ال Result لل Attacker.
ال DOM XSS ايه هي و بتتنفذ ازاي ؟!
اول حاجة ايه هي ال DOM دة اصلاً ؟! ال DOM Environment دي الي هي Document Object Model دة عبارة عن API موجود في ال HTML و ال XML Documents وظيفته انه بيعرف ال Logical Structure بتاع ال documents و ازاي بيحصل Access و Manipulation علي ال Documents دي يعني ازاي الناس بتفتح ال Files دي و تعدل فيها او تجيب ال Data منها.
اصعب حاجة في ال Attack دة بجانب انه بيعتبر Advanced XSS Technique انه كمان مش بيروح يتنفذ في ال Back-end يعني مش بيروح ال Server دة بيتعمل علي ال Client Side عن طريق ال Data الي بتطلع output من ال DOM يعني ال Request و ال Response مبيتغيروش ف اي حاجة علي عكس ال Reflected و ال Stored XSS....ال DOM XSS Payload صعب اوي ان اي حد يعمله Detect ولا حتي ال WAFs لانه مش بيغير حاجة في ال Request ولا ال Response ولا ال Payload بيروح يشتغل علي ال Server اصلا تاني حاجة في ال DOM XSS ال Hacker او ال Attacker مبيهموش ال Filters وال Security mechanisms الي موجودة في ال Server Side لانه اصلا مش هيتعامل مع ال Server Side.....ال Input في ال DOM XSS اسمه Source و ال Output في ال DOM XSS اسمها Sink .
ملحوظة مهمة :
ال DOM XSS ممكن يبقي Persistent و ممكن يبقي non-Persistent .
فيه بقي Vector جديد او Attack جديد من ال XSS اكتشفه باحث امني اسمه Mario Heiderich ال Attack دة اسمها Mutation XSS او فيه ناس بتسميه mXSS
ال Attack دة كل اعتماده علي ال innerHTML
ايه هو ال innerHTML دة ؟!
دة عبارة عن Function او Feature في ال DOM Environment بتسمحلك انك تغير في Content بتاع Element عن طريق ال ID بتاعه.
لحد كدة مفيش مشاكل....فين ال Mutation XSS بقي ؟!
اولا ال mXSS دي تقدر تعمل Bypass لكتير من ال XSS Filters و ال Sanitizers الموجودين .
ال mXSS تقدر تعمل بقي Direct Manipulation في ال HTML content و تـ Bypass ال DOM Environment.
--------------------------------------------------------------------------------
كدة Part 23 من البوست خلص انا كان لازم بس اتكلم عن ال Vulnerabilities دي قبل ما اتكلم عن ال Web-Evasion Techniques انا مش بقول ان دة كل ال Vulnerabilities الي موجودة في ال Web اكيد لا طبعاً انا بس كنت محتاج اقول دول بالذات عشان لما اشرح ال Part الجاي ان شاء الله الصورة تكون واضحة كدة انا شرحت اكتر Critical Vulnerabilities انا محتاجهم ال Part دة شرحت 6 و في Part 12 شرحت ال SQLi ف كدة ان شاء الله اقدر اتكلم عن ال Evasions و تكون الصورة واضحة عند الناس ان شاء الله..... الي عايز يرجع للـ Parts الي قبل كدة دي اللينكات بتاعتهم :
https://pastebin.com/cj9FEuEB
-------------------------------------------------------------------------------
قال رسول الله صلى الله عليه وسلم { من سئل عن علم فكتمه ألجمه الله بلجام من نار يوم القيامة } صدق رسول الله صلى الله عليه وسلم اللهم علمنا ما ينفعنا و انفعنا بما علمتنا و اجعلنا سبب في قضاء حوائج عبادك....
