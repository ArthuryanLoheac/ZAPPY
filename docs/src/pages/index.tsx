import type {ReactNode} from 'react';
import clsx from 'clsx';
import Link from '@docusaurus/Link';
import useDocusaurusContext from '@docusaurus/useDocusaurusContext';
import Layout from '@theme/Layout';
import HomepageFeatures from '@site/src/components/HomepageFeatures';
import Heading from '@theme/Heading';

import styles from './index.module.css';

function HomepageHeader() {
  const {siteConfig} = useDocusaurusContext();
  return (
    <header className={clsx('hero hero--primary', styles.heroBanner)}>
      <div className="container">
        <Heading as="h1" className="hero__title">
          {siteConfig.title}
        </Heading>
        <p className="hero__subtitle">{siteConfig.tagline}</p>
        <div className={styles.buttons}>
          <Link
            className="button button--secondary button--lg"
            to="/docs/zappy/CONTRIBUTING">
            Zoup Zoup !
          </Link>
        </div>
      </div>
    </header>
  );
}

export default function Home(): ReactNode {
  const {siteConfig} = useDocusaurusContext();
  return (
    <Layout
      title={`Hello from ${siteConfig.title}`}
      description="Description will go into a meta tag in <head />">
      <HomepageHeader />
      <main>
        <div style={{justifyItems: 'center', padding: '5%'}}>
          <h1>Hello <del>diddy</del> <em>Zappytto</em> !</h1>
          <p>This website is written in React, so if you don't know it, sucks to be you :p</p>
          <p>
            But don't worry ! <br />
            You can just take a look at the <Link href='/docs/tutorial/intro'>docusorus tutorial</Link> to learn how to contribute to the doc !<br/>
            <i>ps: a link to this tuto is located at the top right of the website, next to the darkmode controller.</i>
          </p>
          <p>
            And if you still don't understand or just don't want to, throw some markdown files under /docs/docs/zappy/ and it should work.
          </p>
        </div>
        <div style={{marginTop: 1000, padding: '10%'}}>
          <p>This is pointless but still, thanks for your attention &lt;3 </p>
        </div>
        <div style={{marginTop: 50000, padding: '10%'}}>
          <p>
            Wow !<br />
            You actually scrolled all the way down here !<br />
            Here is your reward :
          </p>
          <iframe
              src='https://www.google.com/url?sa=i&url=https%3A%2F%2Furbania.ca%2Farticle%2Fcomment-shrek-est-devenu-licone-dune-generation&psig=AOvVaw1h4hRTdCgN0YveP2O_Nv4J&ust=1737927441456000&source=images&cd=vfe&opi=89978449&ved=0CBQQjRxqFwoTCPi7orLqkYsDFQAAAAAdAAAAABAE'
              style={{width: '100%', height: 500}}
            />
        </div>
      </main>
    </Layout>
  );
}
